/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 16:42:56 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/24 18:57:39 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"

char	*extract_expansion_name(char *str)
{
	size_t	i;
	char	*name;

	i = 1;
	name = NULL;
	if (str[i] == '?')
	{
		name = ft_strdup("?");
		if (!name)
		{
			free_executable();
			ft_error_exit("ft_strdup (extract_expansion_name)", ENOMEM);
		}
		return (name);
	}
	while (str[i] && str[i] != '$' && !ft_isspace(str[i]))
		i++;
	name = ft_strndup(str + 1, i - 1);
	if (!name)
	{
		free_executable();
		ft_error_exit("ft_strndup (extract_expansion_name)", ENOMEM);
	}
	return (name);
}

static char	*create_new_string(char *substr_start, char *name, char *value,
	size_t i)
{
	char	*new_str;
	size_t	len;

	if (value)
		len = ft_strlen(g_master.exec->argv[i]) + ft_strlen(value) + 1;
	else
		len = ft_strlen(g_master.exec->argv[i]) + 1;
	new_str = malloc(len);
	if (!new_str)
	{
		free(name);
		free_executable();
		ft_error_exit("malloc (create_new_string)", ENOMEM);
		exit(EXIT_FAILURE);
	}
	ft_strlcpy(new_str, g_master.exec->argv[i], substr_start
		- g_master.exec->argv[i] + 1);
	if (value)
	{
		ft_strlcat(new_str, value, len);
		ft_strlcat(new_str, substr_start + ft_strlen(name) + 1, len);
	}
	else
		ft_strlcat(new_str, substr_start + ft_strlen(name) + 1, len);
	return (new_str);
}

static void	process_expansion_replace(t_exec *exec, char *substr_start, int i)
{
	char	*name;
	char	*value;
	char	*new_str;

	value = NULL;
	name = NULL;
	new_str = NULL;
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
	{
		value = get_env_value(g_master.env_list, name);
	}
	new_str = create_new_string(substr_start, name, value, i);
	free(exec->argv[i]);
	exec->argv[i] = new_str;
	free(substr_start);
	free(name);
	free(value);
}

void	launch_expansion(t_exec *exec)
{
	char	*substr_start;
	size_t	i;
	size_t	j;

	i = 0;
	substr_start = NULL;
	if (!exec || !exec->argv)
		return ;
	while (exec->argv[i])
	{
		j = 0;
		while (exec->argv[i][j])
		{
			if (exec->argv[i][j] == '$')
			{
				substr_start = ft_strdup(exec->argv[i] + j);
				process_expansion_replace(exec, substr_start, i);
			}
			j++;
		}
		i++;
	}
}
