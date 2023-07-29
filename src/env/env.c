/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 16:27:12 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/28 12:42:57 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"
#include "exec.h"
#include "exit.h"
#include "libft.h"

static char	*update_shlvl(char *value, char *name)
{
	int	new_value;

	new_value = ft_atoi(value);
	free(value);
	new_value++;
	value = ft_itoa(new_value);
	if (!value)
	{
		free(name);
		ft_error_exit("ft_itoa (update_shlvl)", ENOMEM);
	}
	return (value);
}

static void	create_add_env_node(char *name, char *value, t_env **env_list)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		free(name);
		free(value);
		ft_error_exit("malloc (create_add_env_node)", ENOMEM);
		return ;
	}
	new_node->name = name;
	new_node->value = value;
	new_node->next = NULL;
	if (!*env_list)
	{
		*env_list = new_node;
		(*env_list)->last = new_node;
	}
	else
	{
		(*env_list)->last->next = new_node;
		(*env_list)->last = new_node;
	}
}

static void	fill_env_node(t_env **env_list, char *n, char *v)
{
	char	*name;
	char	*value;

	name = ft_strdup(n);
	if (ft_strcmp(name, "PWD") == 0)
		value = getcwd(NULL, 0);
	else
		value = ft_strdup(v);
	if (!name || !value)
	{
		free(name);
		free(value);
		ft_error_exit("ft_strdup (manage_empty_environment)", ENOMEM);
		exit(EXIT_FAILURE);
	}
	create_add_env_node(name, value, env_list);
}

static void	manage_empty_environment(t_env **env_list)
{
	fill_env_node(env_list, "PWD", NULL);
	fill_env_node(env_list, "SHLVL", "1");
	fill_env_node(env_list, "_", "minishell");
}

void	manage_environment(t_env **env_list)
{
	char		*name;
	char		*value;
	extern char	**environ;
	char		*equals_location;

	if (!*environ)
		manage_empty_environment(env_list);
	while (*environ)
	{
		equals_location = ft_strchr(*environ, '=');
		name = ft_strndup(*environ, equals_location - *environ);
		if (!name)
			ft_error_exit("ft_strndup (manage_environment)", ENOMEM);
		value = ft_strdup(equals_location + 1);
		if (!value)
		{
			free(name);
			ft_error_exit("ft_strdup (manage_environment)", ENOMEM);
			return ;
		}
		if (!ft_strcmp(name, SHLVL))
			value = update_shlvl(value, name);
		create_add_env_node(name, value, env_list);
		environ++;
	}
	g_master.export_envp = env_list_to_array(g_master.env_list);
}
