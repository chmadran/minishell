/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 14:12:20 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/29 15:19:13 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"
#include "utils.h"

static int	prep_command_or_error(t_exec *exec, t_builtin_type type)
{
	if (type == T_ERROR)
	{
		printf("minishell: %s: command not found\n", exec->argv[0]);
		g_master.exit_status = 127;
		free_executable();
		ft_exit(exec->argc, exec->argv);
	}
	if (check_directory(exec->argv[0]) == EXIT_FAILURE)
		return (T_ERROR);
	exec->pathname = search_pathname_command(exec->argv[0]);
	if (!exec->pathname)
		return (prepare_command(&g_master, exec));
	return (T_OTHERS);
}

static int	prepare_type_execution(t_master *master, t_builtin_type type)
{
	if (type == T_OTHERS)
		type = prep_command_or_error(master->exec, type);
	if (type == T_ERROR)
	{
		free_executable();
		return (EXIT_FAILURE);
	}
	if (type != T_OTHERS && type != T_ERROR && master->token_count == 1
		&& check_redir(master->exec->argv) == -1)
	{
		execute_builtin(master->exec, type);
		free_executable();
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static t_builtin_type	prepare_execution(t_master *master, t_token *token,
	t_exec *exec)
{
	(void)exec;
	(void)token;
	master->exec = create_arguments(token);
	launch_heredoc(master->exec);
	if (!master->exec->argv[0])
		return (T_ERROR);
	return (find_arg_type(master->exec->argv[0]));
}

void	launch_execution(t_master *master)
{
	t_exec			*exec;
	t_token			*token;
	t_builtin_type	type;

	exec = NULL;
	token = master->token_list;
	init_pids();
	if (launch_expansion() == EXIT_FAILURE)
		return ;
	while (token)
	{
		type = prepare_execution(master, token, exec);
		if (prepare_type_execution(master, type) == EXIT_FAILURE)
			return ;
		if (token->next && token->next->type == T_PIPE)
			pipe(master->pipefd);
		master->pid = fork();
		(signal(SIGINT, &child_sigint), signal(SIGQUIT, &child_sigint));
		child_process_execution(master, token, exec, type);
		parent_process_execution(master, &token, exec);
	}
	fd_close(master->pipefd[0]);
	fd_close(master->pipefd[1]);
	wait_all_processes(master);
}
