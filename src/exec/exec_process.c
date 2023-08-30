/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 16:18:49 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/30 18:43:45 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"
#include "utils.h"

void	execve_execute_command(t_exec *exec, t_env *env_list,
			t_builtin_type type)
{
	char	**envp;

	envp = env_list_to_array(env_list);
	if (type == T_OTHERS)
		execve(exec->pathname, exec->argv, envp);
	else
	{
		execute_builtin(exec, type);
		free_double_ptr(envp);
		return ;
	}
	free_double_ptr(envp);
	free_executable();
	perror("execve (execute_command)");
}

void	launch_dup_child_process(t_master *master, t_token *token)
{
	(void)master;
	if (token->next && token->next->type == T_PIPE)
	{
		close(g_master.pipefd[0]);
		dup2(g_master.pipefd[1], STDOUT_FILENO);
		close(g_master.pipefd[1]);
	}
	else if (!g_master.first_cmd)
	{
		close(g_master.pipefd[1]);
		dup2(g_master.pipefd[0], STDIN_FILENO);
		close(g_master.pipefd[0]);
	}
}

void	child_process_execution(t_master *master, t_token *token, t_exec *exec,
			t_builtin_type type)
{
	exec = master->exec;
	if (master->pid == 0)
	{
		dup2(master->tmp_fd, STDIN_FILENO);
		close(master->tmp_fd);
		launch_dup_child_process(master, token);
		if (launch_redirection(exec) == EXIT_FAILURE)
		{
			g_master.exit_status = 1;
			ft_free_child();
			exit(g_master.exit_status);
		}
		execve_execute_command(master->exec, master->env_list, type);
		ft_free_child();
		exit(master->exit_status);
	}
}

void	parent_process_execution(t_master *master, t_token **token)
{
	if (g_master.exit_status != 127 && master->pid > 0)
	{
		master->child_pid[master->count_pid++] = master->pid;
		if ((*token)->next && (*token)->next->type == T_PIPE)
			master->first_cmd = false;
		else
			master->first_cmd = true;
		close(master->tmp_fd);
		if (g_master.pipefd[1] != -1)
			close(g_master.pipefd[1]);
		master->tmp_fd = master->pipefd[0];
		if ((*token)->next)
			*token = (*token)->next->next;
		else
			*token = NULL;
		free_executable();
	}
}
