/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 11:06:41 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/28 14:31:18 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"

static int	open_and_dup(t_exec *exec, int flags, int std_type)
{
	//int	fd;
	int	file;

	file = find_redirection(exec->argv);
	if (std_type == 1)
		g_master.redir_fd_in = open(exec->argv[file + 1], flags, 0644);
	else if (std_type == 0)
		g_master.redir_fd_out = open(exec->argv[file + 1], flags, 0644);
	if (g_master.redir_fd_in < 0 || g_master.redir_fd_out < 0)
	{
		if (access(exec->argv[file + 1], F_OK) == 0)
		{
			printf("minishell: %s: Permission denied\n", exec->argv[0]);
			return (g_master.exit_status = 1, -1);
		}
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(exec->argv[file + 1], STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		g_master.exit_status = 2;
		return (-1);
	}
	// if (redir < 4)
	// 	dup2(fd, std_type);
	// close(fd);
	return (0);
}

int	count_redir(t_exec *exec)
{
	int	i;
	int	j;
	int	count;

	i = 0;
	j = 0;
	count = 0;
	while (exec->argv[i])
	{
		j = 0;
		while (exec->argv[i][j])
		{
			if (exec->argv[i][j] == '<' || exec->argv[i][j] == '>')
				count++;
			j++;
		}
		i++;
	}
	return (count);
}

void	ft_close(int fd)
{
	if (fd > 0)
		close(fd);
}

int	launch_redirection(t_exec *exec)
{
	int	redir;
	int	redir_count;

	redir = check_redir(exec->argv);
	if (redir == -1)
			return (0);
	redir_count = count_redir(exec);
	if (redir == -1)
			return (0);
	while (redir_count > 0)
	{
		redir = check_redir(exec->argv);
		if (redir == -1)
			return (EXIT_SUCCESS);
		if (redir == 1)
		{
			if (open_and_dup(exec, O_RDONLY, 1) == -1)
			{
				clean_argv(exec);
				return (EXIT_FAILURE);
			}
		}
		else if (redir == 2 || redir == 4)
		{
			if (open_and_dup(exec, O_WRONLY | O_CREAT | O_TRUNC, 0) == -1)
			{
				return (EXIT_FAILURE);
			}
		}
		else if (redir == 3 || redir == 5)
		{
			if (open_and_dup(exec, O_WRONLY | O_CREAT | O_APPEND, 0) == -1)
			{
				return (EXIT_FAILURE);
			}
		}
		clean_argv(exec);
		redir_count = count_redir(exec);
	}
	if (redir < 4)
	{
		if (g_master.redir_fd_in > 0)
			dup2(g_master.redir_fd_in, STDIN_FILENO);
		if (g_master.redir_fd_out > 0)
			dup2(g_master.redir_fd_out, STDOUT_FILENO);
	}
	ft_close(g_master.redir_fd_in);
	ft_close(g_master.redir_fd_out);
	return (EXIT_SUCCESS);
}
