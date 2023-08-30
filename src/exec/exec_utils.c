/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 10:37:01 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/30 14:19:52 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"
#include "utils.h"

int	prep_command_or_error(t_exec *exec, t_builtin_type type)
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

void	wait_all_processes(t_master *master)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (i < master->count_pid)
	{
		waitpid(master->child_pid[i], &status, 0);
		if (WIFEXITED(status) && g_master.exit_status != 127)
			g_master.exit_status = WEXITSTATUS(status);
		master->child_pid[i] = -1;
		i++;
	}
}

void	init_pids(void)
{
	g_master.pipefd[0] = -1;
	g_master.pipefd[1] = -1;
	g_master.pid = -1;
	g_master.count_pid = 0;
	g_master.first_cmd = true;
	g_master.tmp_fd = dup(STDIN_FILENO);
}
