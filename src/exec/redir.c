/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 11:06:41 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/23 18:01:35 by chmadran         ###   ########.fr       */
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

	fd = open(exec->argv[exec->argc - 1], flags, 0644);
	if (fd < 0)
		return (-1);
	if (redir > 0)
		dup2(fd, std_type);
	close(fd);
	return (0);
}

int	launch_redirection(t_exec *exec)
{
	int	redir;

	redir = check_redir(exec->argv);
	while (redir != -1)
	{
		if (redir == 1)
			open_and_dup(exec, O_RDONLY, STDIN_FILENO, redir);
		else if (redir == 2 || redir == 0)
			open_and_dup(exec, O_WRONLY | O_CREAT | O_TRUNC, STDOUT_FILENO, redir);
		else if (redir == 3 || redir == 0)
			open_and_dup(exec, O_WRONLY | O_CREAT | O_APPEND, STDOUT_FILENO, redir);
		clean_argv(exec);
		redir = check_redir(exec->argv);
	}
	if (!exec->argv)
		return(0);
	return (1);
}
