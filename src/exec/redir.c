/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 11:06:41 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/28 13:35:17 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"

static int	open_and_dup(t_exec *exec, int flags, int std_type, int redir)
{
	int	fd;
	int	file;

	file = find_redirection(exec->argv);
	fd = open(exec->argv[file + 1], flags, 0644);
	if (fd < 0)
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
	if (redir < 4)
		dup2(fd, std_type);
	close(fd);
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
		if (redir == 1)
		{
			if (open_and_dup(exec, O_RDONLY, STDIN_FILENO, redir) == -1)
			{
				clean_argv(exec);
				return (EXIT_FAILURE);
			}
		}
		else if (redir == 2 || redir == 4)
		{
			if (open_and_dup(exec, O_WRONLY | O_CREAT | O_TRUNC, STDOUT_FILENO, redir) == -1)
			{
				return (EXIT_FAILURE);
			}
		}
		else if (redir == 3 || redir == 5)
		{
			if (open_and_dup(exec, O_WRONLY | O_CREAT | O_APPEND, STDOUT_FILENO, redir) == -1)
			{
				return (EXIT_FAILURE);
			}
		}
		clean_argv(exec);
		redir_count = count_redir(exec);
	}
	return (EXIT_SUCCESS);
}
