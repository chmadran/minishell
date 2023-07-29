/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 12:00:21 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/27 18:39:32 by chmadran         ###   ########.fr       */
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
		*equals_location = '\0';
		var->name = ft_strdup(var_str);
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

int	ft_export(int argc, char **argv)
{
	int		i;
	t_env	*current;
	char	*equals_location;

	i = 0;
	current = g_master.env_list;
	equals_location = NULL;
	if (argc == 1)
	{
		while (current)
		{
			printf("export %s=\"%s\"\n", current->name, current->value);
			current = current->next;
		}
		return (EXIT_SUCCESS);
	}
	while (++i < argc)
	{
		if (ft_strlen(argv[i]) && argv[i][0] != '=')
			equals_location = ft_strchr(argv[i], '=');
		if (export_var(argv[i], equals_location))
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
