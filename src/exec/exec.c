/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 14:12:20 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/29 15:15:36 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"

static int	prep_command_or_error(t_exec *exec, t_builtin_type type)
{
	if (type == T_ERROR)
	{
		printf("minishell: %s: command not found\n", exec->argv[0]);
		g_master.exit_status = 127;
		free_executable();
		ft_exit(exec->argc, exec->argv);
	}
	return (prepare_command(&g_master, exec));
}

static t_builtin_type	find_arg_type(char *arg)
{
	size_t					i;
	t_builtin_type			type;
	const char				*builtins[8] = {"cd", "echo", "env", "exit", "pwd",
		"unset", "export", "exit"};
	const t_builtin_type	types[8] = {T_CD, T_ECHO, T_ENV, T_EXIT, T_PWD,
		T_UNSET, T_EXPORT, T_EXIT};

	if (!arg || !arg[0])
		return (T_ERROR);
	i = 0;
	type = T_OTHERS;
	while (i < 8 && *builtins[i])
	{
		if (!ft_strcmp(builtins[i], arg))
		{
			type = types[i];
			break ;
		}
		i++;
	}
	return (type);
}

static t_builtin_type	prepare_execution(t_master *master, t_token *token)
{
	master->exec = create_arguments(token);
	launch_expansion(master->exec);
	return (find_arg_type(master->exec->argv[0]));
}

void	launch_execution(t_master *master)
{
	t_exec			exec;
	int				status;
	t_token			*token;
	t_builtin_type	type;

	exec.pipefd[0] = -1;
	exec.pipefd[1] = -1;
	exec.old_pipefd[0] = -1;
	exec.old_pipefd[1] = -1;
	exec.pid = -1;
	exec.first_cmd = true;
	token = master->token_list;
	while (token)
	{
		type = prepare_execution(master, token);
		if (type == T_OTHERS)
			type = prep_command_or_error(master->exec, type);
		if (type == T_ERROR)
		{
			free_executable();
			return ;
		}
		if (type != T_OTHERS && type != T_ERROR && master->token_count == 1)
		{
			g_master.exit_status = execute_builtin(master->exec, type);
			free_executable();
			return ;
		}
		if (token->next && token->next->type == T_PIPE)
			pipe(exec.pipefd);
		exec.pid = fork();
		child_process_execution(master, token, &exec, type);
		parent_process_execution(&token, &exec);
	}
	if (!exec.first_cmd)
	{
		close(exec.old_pipefd[0]);
		close(exec.old_pipefd[1]);
	}
	while ((waitpid(exec.pid, &status, 0)) > 0)
		if (WIFEXITED(status) && master->exit_status != 127)
			master->exit_status = WEXITSTATUS(status);
}
