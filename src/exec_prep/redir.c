/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 11:06:41 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/31 17:24:16 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"
#include "utils.h"

int	error_redir(t_exec *exec, int file)
{
	if (access(exec->argv[file + 1], F_OK) == 0)
	{
		printf("minishell: %s: Permission denied\n", exec->argv[0]);
		g_master.exit_status = 1;
	}
	else
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(exec->argv[file + 1], STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		g_master.exit_status = 2;
	}
	return (-1);
}

static int	open_and_dup(t_exec *exec, int std_type, int redir)
{
	int	file;
	int	flags;

	if (redir == 1)
		flags = O_RDONLY;
	if (redir == 2 || redir == 4)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	if (redir == 3 || redir == 5)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	file = find_redirection(exec->argv);
	if (std_type == 1)
		g_master.redir_fd_in = open(exec->argv[file + 1], flags, 0644);
	else if (std_type == 0)
		g_master.redir_fd_out = open(exec->argv[file + 1], flags, 0644);
	if (g_master.redir_fd_in < 0 || g_master.redir_fd_out < 0)
	{
		return (error_redir(exec, file));
	}
	return (0);
}

int	dup_close_redir(int redir)
{
	if (redir < 4)
	{
		if (g_master.redir_fd_in > 0)
			dup2(g_master.redir_fd_in, STDIN_FILENO);
		if (g_master.redir_fd_out > 0)
			dup2(g_master.redir_fd_out, STDOUT_FILENO);
	}
	fd_close(g_master.redir_fd_in);
	fd_close(g_master.redir_fd_out);
	return (EXIT_SUCCESS);
}

int	launch_open_dup(int redir, t_exec *exec)
{
	if (redir == 1)
	{
		if (open_and_dup(exec, 1, redir) == -1)
		{
			clean_argv(exec);
			return (EXIT_FAILURE);
		}
	}
	else if (redir > 1)
	{
		if (open_and_dup(exec, 0, redir) == -1)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	launch_redirection(t_exec *exec)
{
	int	redir;
	int	redir_count;

	redir = check_redir(exec->argv);
	if (redir == -1)
		return (0);
	redir_count = count_redir(exec);
	while (redir_count > 0)
	{
		redir = check_redir(exec->argv);
		if (redir == -1)
		{
			dup_close_redir(redir);
			return (EXIT_SUCCESS);
		}
		if (launch_open_dup(redir, exec) == EXIT_FAILURE
			|| clean_argv(exec) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		redir_count = count_redir(exec);
	}
	return (dup_close_redir(redir));
}
