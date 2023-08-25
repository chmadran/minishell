/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 11:06:41 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/25 13:49:38 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"

int	find_redirection(char **argv)
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
	int		i;
	int		j;
	int		k;
	char	**new_argv;

	i = 0;
	k = 0;
	j = find_redirection(exec->argv);
	if (j == -1)
		return (EXIT_FAILURE);
	new_argv = malloc(sizeof(char *) * (exec->argc + 1));
	while (exec->argv[i] && i < j)
		new_argv[k++] = ft_strdup(exec->argv[i++]);
	i = i + 2;
	exec->argc = i;
	while (exec->argv[i])
		new_argv[k++] = ft_strdup(exec->argv[i++]);
	new_argv[k] = NULL;
	free_double_ptr(exec->argv);
	exec->argv = new_argv;
	exec->argc = k;
	return (EXIT_SUCCESS);
}

int	check_redir(char **argv)
{
	int		i;
	size_t	j;
	char	**rline_av;

	i = -1;
	j = 0;
	rline_av = ft_spe_split(g_master.line_read, ' ', 0, 0);
	while (argv[++i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] == '<' && !(is_in_quotes(rline_av[i], j + 1)))
			{
				return (1);
			}
			else if (argv[i][j] == '>' && !(is_in_quotes(rline_av[i], j + 1)))
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
	}
	free_double_ptr(rline_av);
	return (-1);
}
