/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 11:40:56 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/29 16:15:52 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"

static int	update_env(t_env *env_list)
{
	t_env	*current;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ft_error_exit("getcwd", ERANGE);
		return (EXIT_FAILURE);
	}
	current = env_list;
	while (current && current->name && ft_strcmp(current->name, "PWD"))
		current = current->next;
	if (!current)
	{
		free(cwd);
		return (EXIT_FAILURE);
	}
	free(current->value);
	current->value = ft_strdup(cwd);
	if (!current->value)
		return (EXIT_FAILURE);
	free(cwd);
	return (EXIT_SUCCESS);
}

static int	change_directory_and_update(t_env *env_list, char *path)
{
	if (path[0] == '/')
		path++;
	if (chdir(path) == -1)
	{
		printf("minishell: cd: %s: No such file or directory\n", path);
		g_master.exit_status = 1;
		return (EXIT_FAILURE);
	}
	if (update_env(env_list))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	ft_cd(int argc, char **argv)
{
	char	*home_path;

	if (argc > 2)
	{
		printf("minishell: cd: too many arguments\n");
		g_master.exit_status = 1;
		return (EXIT_FAILURE);
	}
	if (argc == 1)
	{
		home_path = get_env_value(g_master.env_list, "HOME", 0);
		if (!home_path)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		return (change_directory_and_update(g_master.env_list, home_path));
	}
	if (check_option(argv[1], 0))
		return (EXIT_FAILURE);
	return (change_directory_and_update(g_master.env_list, argv[1]));
}
