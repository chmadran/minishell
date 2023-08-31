/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 12:30:45 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/31 19:45:52 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"
#include "utils.h"

static int	is_valid_identifier(char *str)
{
	int	i;

	i = 0;
	if (!ft_isalpha(str[0]) && str[0] != '_')
	{
		printf("minishell: unset: '%s': not a valid identifier\n", str);
		g_master.exit_status = 1;
		return (EXIT_FAILURE);
	}
	while (str[i])
	{
		if (!ft_isalpha(str[i]) && !ft_isdigit(str[i]) && str[i] != '_')
		{
			printf("minishell: unset: '%s': not a valid identifier\n", str);
			g_master.exit_status = 1;
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

static void	delete_i_from_export_envp(int i)
{
	int		j;
	int		k;
	char	**new_argv;

	j = 0;
	k = 0;
	new_argv = malloc(sizeof(char *) * (g_master.size_export_envp) + 1);
	while (g_master.export_envp[j] && j < i)
		new_argv[k++] = ft_strdup(g_master.export_envp[j++]);
	if (g_master.export_envp[j])
	{
		j++;
		while (g_master.export_envp[j])
			new_argv[k++] = ft_strdup(g_master.export_envp[j++]);
	}
	new_argv[k] = NULL;
	free_double_ptr(g_master.export_envp);
	g_master.export_envp = new_argv;
	g_master.size_export_envp = ft_array_size(g_master.export_envp);
}

void	unset_from_export_env(char *argv)
{
	int		i;
	char	*name;
	char	*equals_location;

	i = -1;
	while (g_master.export_envp[++i])
	{
		equals_location = ft_strchr(g_master.export_envp[i], '=');
		name = ft_strndup(g_master.export_envp[i],
				equals_location - g_master.export_envp[i]);
		if (ft_strncmp(argv, g_master.export_envp[i], ft_strlen(name)) == 0)
		{
			delete_i_from_export_envp(i);
			free(name);
			return ;
		}
		free(name);
	}
}

int	ft_unset(int argc, char **argv)
{
	int		i;
	t_env	*current;

	(void)argc;
	i = 0;
	current = g_master.env_list;
	while (argv[++i])
	{
		current = g_master.env_list;
		if (check_event(argv[i], 1) || is_valid_identifier(argv[i]))
			continue ;
		while (current)
		{
			unset_from_export_env(argv[i]);
			if (ft_strcmp(argv[i], current->name) == 0)
			{
				remove_var(&g_master, current);
				break ;
			}
			current = current->next;
		}
	}
	return (EXIT_SUCCESS);
}
