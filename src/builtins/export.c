/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 12:00:21 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/25 13:17:05 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"

static int	export_var(char *var_str, char *equals_location)
{
	t_env	*var;
	bool	status;

	var = malloc(1 * sizeof(t_env));
	if (!var)
		return (EXIT_FAILURE);
	if (equals_location)
	{
		var->name = ft_strndup(var_str, ft_strlen(var_str)
				- ft_strlen(equals_location));
		var->value = ft_strdup(equals_location + 1);
	}
	else
	{
		var->name = ft_strdup(var_str);
		var->value = ft_strdup("");
	}
	status = is_var_already_in_env(&g_master.env_list, var);
	if (status)
		return (free(var), EXIT_SUCCESS);
	var->next = NULL;
	if (!is_valid_variable_name(var->name, var_str))
		return (free(var->name), free(var->value), free(var), EXIT_FAILURE);
	add_back_env_var(&g_master.env_list, var);
	return (EXIT_SUCCESS);
}

static char	**add_back_envp_var(char *argv, char **envp)
{
	int		i;
	char	**new_array;

	i = 0;
	new_array = malloc((g_master.size_export_envp + 2) * sizeof(char *));
	if (!new_array)
		return (NULL);
	while (envp[i])
	{
		new_array[i] = ft_strdup(envp[i]);
		i++;
	}
	new_array[i] = ft_strdup(argv);
	new_array[i + 1] = NULL;
	return (new_array);
}

int	export_local_var(char *argv)
{
	char	**new_array;

	new_array = add_back_envp_var(argv, g_master.export_envp);
	if (!new_array)
		return (EXIT_FAILURE);
	free_double_ptr(g_master.export_envp);
	g_master.export_envp = new_array;
	g_master.size_export_envp++;
	return (EXIT_SUCCESS);
}

int	check_export(int argc, char **argv, int array_size)
{
	int	i;

	i = 0;
	g_master.export_envp = ft_sort_array(array_size, g_master.export_envp);
	if (argc == 1)
		return (print_export(g_master.export_envp));
	while (++i < argc)
	{
		if (check_equals(argv[i]) || check_event(argv[i], 0)
			|| check_option(argv[i], 1))
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	ft_export(int argc, char **argv)
{
	int		i;
	//int		array_size;
	char	*equals_location;

	i = 0;
	g_master.size_export_envp = ft_array_size(g_master.export_envp);
	if (check_export(argc, argv, g_master.size_export_envp) == EXIT_SUCCESS)
	{
		while (++i < argc)
		{
			equals_location = ft_strchr(argv[i], '=');
			if (equals_location && *(equals_location + 1))
			{
				if (export_var(argv[i], equals_location))
					return (EXIT_FAILURE);
			}
			if (ft_strlen(argv[i])
				&& is_valid_variable_name(argv[i], argv[i]))
			{
				if (export_local_var(argv[i]))
					return (EXIT_FAILURE);
			}
		}
	}
	return (EXIT_SUCCESS);
}
