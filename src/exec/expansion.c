/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 16:42:56 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/21 13:40:04 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"

static char	*extract_expansion_name(char *str)
{
	size_t	i;
	char	*name;

	i = 1;
	name = NULL;
	if (str[i] == '?')
	{
		name = ft_strdup("?");
		return (name);
	}
	if (ft_isdigit(str[i]))
	{
		name = ft_strndup(&str[i], 1);
		return (name);
	}
	while (str[i] && str[i] != '$' && !ft_isspace(str[i]) && (ft_isalpha(str[i])
			|| str[i] == '_' || ft_isdigit(str[i])))
		i++;
	name = ft_strndup(str + 1, i - 1);
	return (name);
}

static void	process_expansion_replace(t_exec *exec, char *substr_start,
		size_t i, char *str)
{
	char	*name;
	char	*value;
	char	*new_str;

	name = extract_expansion_name(substr_start);
	if (substr_start[1] == '?')
	{
		value = ft_itoa(g_master.exit_status);
		if (!value)
		{
			free(name);
			free_executable();
			ft_error_exit("ft_itoa (process_expansion)", ENOMEM);
			exit(EXIT_FAILURE);
		}
	}
	else
		value = get_env_value(g_master.env_list, name, 1);
	new_str = create_new_string(substr_start, name, value, str);
	free(exec->argv[i]);
	exec->argv[i] = new_str;
	free(substr_start);
	free(name);
	free(value);
}

void	realocate_argv(t_exec *exec, int i)
{
	int		j;
	char	*new_av;
	size_t	k;

	j = 0;
	k = 0;
	new_av = malloc(sizeof(char) * (ft_strlen(exec->argv[i])));
	while (k < ft_strlen(exec->argv[i]))
	{
		if (exec->argv[i][k] == '$' && exec->argv[i][k + 1] == '\'')
		{
			k += 2;
			continue ;
		}
		if (exec->argv[i][k] == '\'')
		{
			k++;
			continue ;
		}
		new_av[j++] = exec->argv[i][k++];
	}		
	new_av[j] = '\0';
	free(exec->argv[i]);
	exec->argv[i] = ft_strdup(new_av);
	free(new_av);
}

static	void	execute_process_exp(t_exec *exec, int i, int j)
{
	char	*substr_start;

	if ((j == 0 && exec->argv[i][j + 1])
		|| (j != 0 && exec->argv[i][j - 1] != '\\'))
	{
		substr_start = ft_strdup(exec->argv[i] + j);
		if (is_valid_name(substr_start) == 0)
			process_expansion_replace(exec, substr_start, i,
				exec->argv[i]);
		else
			free(substr_start);
	}
}

void	launch_expansion(t_exec *exec)
{
	size_t	i;
	size_t	j;

	i = -1;
	while (exec->argv[++i])
	{
		j = -1;
		while (exec->argv[i][++j])
		{
			if (exec->argv[i][j] == '\'')
			{
				realocate_argv(exec, i);
				break ;
			}
			else if (exec->argv[i][j] == '$')
			{
				if (exec->argv[i][j] == '\'')
					realocate_argv(exec, i);
				else
					execute_process_exp(exec, i, j);
			}
		}
	}
}
