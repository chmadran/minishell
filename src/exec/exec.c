/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 14:12:20 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/25 17:08:36 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"

static int	launch_command_or_builtin(t_exec *exec, t_builtin_type type)
{
	if (type == T_ERROR)
	{
		printf("minishell: %s: command not found\n", exec->argv[0]);
		g_master.exit_status = 127;
		return (T_ERROR);
	}
	else if (type != T_OTHERS)
	{
		g_master.exit_status = execute_builtin(exec, type);
		return (type);
	}
	else if (!execute_command(&g_master, exec))
		return (T_ERROR);
	return (T_OTHERS);
}

static t_builtin_type	find_arg_type(char *arg)
{
	size_t					i;
	t_builtin_type			type;
	const char				*builtins[8] = {"cd", "echo", "env", "exit", "pwd",
		"unset", "export", "exit"};
	const t_builtin_type	types[8] = {T_CD, T_ECHO, T_ENV, T_EXIT, T_PWD,
		T_UNSET, T_EXPORT, T_EXIT};

	if (!arg)
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

static void	prepare_execution(t_master *master, t_token *token, t_exec *exec)
{
	t_builtin_type	type;

	(void)exec;
	master->exec = create_arguments(token);
	launch_expansion(master->exec);
	type = find_arg_type(master->exec->argv[0]);
	launch_command_or_builtin(master->exec, type);
	if (type == T_ERROR || master->exit_status == 127)
	{
		free_executable();
		return ;
	}
	if (type == T_OTHERS)
	{
		if (token->next && token->next->type == T_PIPE)
			if (pipe(exec->pipefd) == -1)
				perror("pipe (execute_pipeline)");
		exec->pid = fork();
		if (exec->pid == -1)
			perror("fork (execute_pipeline)");
	}
	return ;
}

void	launch_execution(t_master *master)
{
	t_exec		exec;
	int			status;
	t_token		*token;

	exec.pipefd[0] = -1;
	exec.pipefd[1] = -1;
	exec.old_pipefd[0] = -1;
	exec.old_pipefd[1] = -1;
	exec.pid = -1;
	exec.first_cmd = true;
	token = master->token_list;
	while (token)
	{
		prepare_execution(master, token, &exec);
		if (master->exit_status == 127)
			break ;
		//print_executable(&exec);
		child_process_execution(master, token, &exec);
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
