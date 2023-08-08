/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 11:06:41 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/29 16:11:05 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"

int	open_and_dup(t_exec *exec, int flags, int std_type)
{
	int	fd;

	fd = open(exec->argv[exec->argc - 1], flags, 0644);
	if (fd < 0)
		return (-1);
	dup2(fd, std_type);
	close(fd);
	return (0);
}

int	launch_redirection(t_exec *exec)
{
	int	redir;

	redir = check_redir(exec->argv);
	if (redir == 1)
	{
		if (open_and_dup(exec, O_RDONLY, STDIN_FILENO) == -1)
		{
			clean_redir(exec);
			return (-1);
		}
	}
	else if (redir ==	2 && open_and_dup(exec, O_WRONLY | O_CREAT | O_TRUNC, STDOUT_FILENO) == - 1)
		return (-1);
	else if (redir == 3 && open_and_dup(exec, O_WRONLY | O_CREAT | O_APPEND, STDOUT_FILENO) == -1)
		return (-1);
	clean_argv(exec);
	return (0);
}
