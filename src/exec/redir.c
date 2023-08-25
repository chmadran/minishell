/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 11:06:41 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/25 10:16:05 by chmadran         ###   ########.fr       */
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
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(exec->argv[file + 1], STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
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

	redir = check_redir(exec->argv);
	if (redir == -1)
		return (0);
	if (redir == 1)
	{
		if (open_and_dup(exec, O_RDONLY, STDIN_FILENO, redir) == -1)
		{
			clean_argv(exec);
			return (EXIT_FAILURE);
		}
	}
	else if (redir == 2 || redir == 4)
		open_and_dup(exec, O_WRONLY | O_CREAT | O_TRUNC, STDOUT_FILENO, redir);
	else if (redir == 3 || redir == 5)
		open_and_dup(exec, O_WRONLY | O_CREAT | O_APPEND, STDOUT_FILENO, redir);
	clean_argv(exec);
	return (EXIT_SUCCESS);
}
