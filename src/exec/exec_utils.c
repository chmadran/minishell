/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 10:37:01 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/29 10:42:37 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"
#include "utils.h"

void	wait_all_processes(t_master *master)
{
	int	i;
	int	status;

	i = 0;
	while (i < master->count_pid)
	{
		waitpid(master->child_pid[i], &status, 0);
		if (WIFEXITED(status) && master->exit_status != 127)
			master->exit_status = WEXITSTATUS(status);
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
