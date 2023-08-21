/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avallet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 17:09:47 by avallet           #+#    #+#             */
/*   Updated: 2023/08/21 17:09:50 by avallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"

void	add_tmp_file(t_exec *exec, int heredoc_tkn)
{
	int		i;
	int		j;
	char	**new_argv;

	i = 1;
	j = 0;
	new_argv = NULL;
	if (ft_strcmp(exec->argv[0], "cat") == 0 && heredoc_tkn == 1)
	{
		new_argv = (char **)malloc(sizeof(char *) * (exec->argc + 2));
		if (!new_argv)
			exit(EXIT_FAILURE);
		new_argv[j++] = ft_strdup("cat");
		new_argv[j++] = ft_strdup("minishell_heredoc_tmp.txt");
		while (exec->argv[i])
		{
			new_argv[j] = exec->argv[i];
			i++;
			j++;
		}
		new_argv[j] = NULL;
		free_double_ptr(exec->argv);
		exec->argv = new_argv;
	}
}

void	clean_args(t_exec *exec, int heredoc_index)
{
	int	i;

	i = heredoc_index;
	free(exec->argv[heredoc_index]);
	free(exec->argv[heredoc_index + 1]);
	while (exec->argv[i + 2])
	{
		exec->argv[i] = exec->argv[i + 2];
		i++;
	}
	exec->argv[i] = NULL;
	exec->argc -= 2;
}

int	check_heredoc(char **argv, int *position)
{
	int	i;
	int	j;

	i = 0;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (ft_strncmp(&argv[i][j], "<<", 2) == 0)
			{
				*position = j;
				return (i);
			}
			j++;
		}
		i++;
	}
	return (-1);
}
