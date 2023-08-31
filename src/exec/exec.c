/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 14:12:20 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/31 14:25:41 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"
#include "utils.h"

static int	prepare_type_execution(t_master *master, t_builtin_type type)
{
	(void)master;
	if (type == T_OTHERS)
		type = prep_command_or_error(g_master.exec, type);
	if (type == T_ERROR)
	{
		free_executable();
		return (EXIT_FAILURE);
	}
	if (type != T_OTHERS && type != T_ERROR && g_master.token_count == 1
		&& check_redir(g_master.exec->argv) == -1)
	{
		execute_builtin(g_master.exec, type);
		free_executable();
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static t_builtin_type	prepare_execution(t_master *master, t_token *token,
	t_exec *exec)
{
	(void)master;
	(void)exec;
	(void)token;
	g_master.exec = create_arguments(token);
	if (!g_master.exec->argv[0])
		return (T_ERROR);
	return (find_arg_type(g_master.exec->argv[0]));
}

static void	execute_proc(t_master *master, t_token **token, t_exec *exec,
	t_builtin_type type)
{
	g_master.exit_status = 0;
	if ((*token)->next && (*token)->next->type == T_PIPE)
		pipe(g_master.pipefd);
	(signal(SIGINT, &child_sigint), signal(SIGQUIT, &child_sigint));
	g_master.pid = fork();
	child_process_execution(master, *token, exec, type);
	parent_process_execution(master, token);
}

static void	token_exec(t_master *master, t_token **token, t_exec *exec)
{
	t_builtin_type	type;

	while (*token)
	{
		type = prepare_execution(master, *token, exec);
		if (launch_heredoc(g_master.exec, master) == EXIT_FAILURE)
		{
			free_executable();
			return ;
		}
		if (prepare_type_execution(master, type) == EXIT_FAILURE)
		{
			if ((*token)->next && (*token)->next->type == T_PIPE)
			{
				g_master.exit_status = 0;
				*token = (*token)->next->next;
				continue ;
			}
			else
				return ;
		}
		execute_proc(master, token, exec, type);
	}
}

void	launch_execution(t_master *master)
{
	t_exec			*exec;
	t_token			*token;

	exec = NULL;
	token = g_master.token_list;
	init_pids();
	if (launch_expansion() == EXIT_FAILURE)
		return ;
	token_exec(master, &token, exec);
	fd_close(g_master.pipefd[0]);
	fd_close(g_master.pipefd[1]);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	wait_all_processes();
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}
