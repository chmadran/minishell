/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 12:00:21 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/29 12:19:53 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"


static char **ft_sort_array(int argc, char **argv)
{
	char *buf;
	int i;

	i = 1;
	while (i < argc)
	{
		while (i < argc - 1)
		{
			if (ft_strcmp(argv[i], argv[i + 1]) > 0)
			{
				buf = argv[i + 1];
				argv[i + 1] = argv[i];
				argv[i] = buf;
			}
			i++;
		}
		i = 1;
		argc--;
	}
	return (argv);
}

static int	export_var(char *var_str, char *equals_location)
{
	t_env	*var;
	bool	status;

	var = malloc(1 * sizeof(t_env));
	if (!var)
		return (EXIT_FAILURE);
	if (equals_location)
	{
		var->name = ft_strndup(var_str, ft_strlen(var_str) - ft_strlen(equals_location));
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

static int print_export(char **envp)
{
	int		i;
	char	*equals_location;
	char	*name;
	char	*value;

	i = 0;
	equals_location = NULL;
	while (envp[i])
	{
		equals_location = ft_strchr(envp[i], '=');
		if (equals_location)
		{
			name = ft_strndup(envp[i], ft_strlen(envp[i]) - ft_strlen(equals_location));
			value = ft_strdup(equals_location + 1);
			printf("export %s=\"%s\"\n", name, value);
			free(value);
		}
		else
		{
			name = ft_strndup(envp[i], ft_strlen(envp[i]));
			printf("export %s=\"\"\n", name);
		}
		free(name);
		i++;
	}
	return(EXIT_SUCCESS);
}

static int	ft_array_size(char **envp)
{
	int size;

	size = 0;
	while (envp[size])
		size++;
	return (size);
}

static char	**add_back_envp_var(int arraySize, char *argv, char **envp)
{
	int		i;
	char	**newArray;

	i = 0;
	newArray = malloc((arraySize + 2) * sizeof(char *));
	if (!newArray)
		return (NULL);
	while(envp[i])
	{
		newArray[i] = ft_strdup(envp[i]);
		i++;
	}
	newArray[i] = ft_strdup(argv);
	newArray[i + 1] = NULL;
	return (newArray);
}

static int	check_event(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '!')
		{
			printf("minishell: export: '%s': event not found\n", &str[i]);
			g_master.exit_status = 1;
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

int	check_equals(char *str)
{
	if (str[0] == '=')
	{
		printf("minishell: export: '%s': not a valid identifier\n", str);
		g_master.exit_status = 1;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	check_option(char *str)
{
	if (str[0] == '-')
	{
		if (str[1])
			printf("minishell: export: '%c%c': invalid option\n", str[0], str[1]);
		else
			printf("minishell: export: '%c': invalid option\n", str[0]);
		g_master.exit_status = 2;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	ft_export(int argc, char **argv)
{
	int		i;
	int		array_size;
	char	**newArray;
	char	*equals_location;

	i = 0;
	equals_location = NULL;
	array_size = ft_array_size(g_master.export_envp);
	g_master.export_envp = ft_sort_array(array_size, g_master.export_envp);
	if (argc == 1)
		return (print_export(g_master.export_envp));
	while (++i < argc)
	{
		if (check_equals(argv[i]) || check_event(argv[i]) || check_option(argv[i]))
			return (EXIT_FAILURE);
		if (ft_strlen(argv[i]))
			equals_location = ft_strchr(argv[i], '=');
		if (equals_location && *(equals_location + 1))
		{
			if (export_var(argv[i], equals_location))
				return (EXIT_FAILURE);
		}
		else if (ft_strlen(argv[i]))
		{
			newArray = add_back_envp_var(array_size, argv[i], g_master.export_envp);
			if (!newArray)
				return (EXIT_FAILURE);
			free_double_ptr(g_master.export_envp);
			g_master.export_envp = newArray;
			array_size++;
		}
	}
	return (EXIT_SUCCESS);
}
