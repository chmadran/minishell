/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 11:06:41 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/24 11:46:51 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"

static int	find_redirection(char **argv)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] == '<' || argv[i][j] == '>')
				return (i);
			j++;
		}
		i++;
	}
	return (-1);
}

int	clean_argv(t_exec *exec)
{
	int	i;

	i = find_redirection(exec->argv);
	if (i == -1)
		return (EXIT_FAILURE);
	exec->argc = i + 1;
	while (exec->argv[i])
	{
		free(exec->argv[i]);
		exec->argv[i] = NULL;
		i++;
	}
	return (EXIT_SUCCESS);
}

int	check_redir(char **argv)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] == '<')
				return (1);
			else if (argv[i][j] == '>')
			{
				if (argv[i][j + 1] && argv[i][j + 1] == '>')
				{
					if (i == 0)
						return (4);
					else
						return (3);
				}
				if (i == 0)
					return (5);
				else
					return (2);
			}
			j++;
		}
		i++;
	}
	return (-1);
}
