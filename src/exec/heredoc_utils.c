/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 17:09:47 by avallet           #+#    #+#             */
/*   Updated: 2023/08/22 09:34:46 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"

void	add_tmp_file(t_exec *exec, int hdoc_tkn)
{
	int		i;
	int		j;
	char	**new_argv;

	i = 1;
	j = 0;
	new_argv = NULL;
	if (exec->argv[0] && (ft_strcmp(exec->argv[0], "cat") == 0 && hdoc_tkn == 1
			&& (!exec->argv[1] || ft_strcmp(exec->argv[1], ">") == 0)))
	{
		new_argv = (char **)malloc(sizeof(char *) * (exec->argc + 2));
		if (!new_argv)
			exit(EXIT_FAILURE);
		new_argv[j++] = ft_strdup("cat");
		new_argv[j++] = ft_strdup("minishell_heredoc_tmp.txt");
		while (exec->argv[i])
			new_argv[j++] = ft_strdup(exec->argv[i++]);
		new_argv[j] = NULL;
		if (g_master.exec->argv)
			free_double_ptr(g_master.exec->argv);
		g_master.exec->argv = new_argv;
		g_master.exec->argc += 1;
	}
	if (!g_master.exec->argv[0] && hdoc_tkn == 1)
	{
		g_master.exec->argv[0] = ft_strdup("cat");
		g_master.exec->argv[1] = ft_strdup("minishell_heredoc_tmp.txt");
		g_master.exec->argc = 2;
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
	int			i;
	int			j;
	char		**tab_readline;

	i = 0;
	tab_readline = ft_spe_split(g_master.line_read, ' ', 0, 0);
	i = 0;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (ft_strncmp(&argv[i][j], "<<", 2) == 0)
			{
				if (is_in_quotes(tab_readline[i], j + 1))
				{
					free_double_ptr(tab_readline);
					return (-1);
				}
					return (-1);
				*position = j;
				return (i);
			}
			j++;
		}
		i++;
	}
	free_double_ptr(tab_readline);
	return (-1);
}
