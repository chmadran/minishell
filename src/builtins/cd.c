/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 11:40:56 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/29 12:21:12 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"

static int	check_option(char *str)
{
	if (str[0] == '-' && str[1])
	{
		if (str[1])
			printf("minishell: unset: '%c%c': invalid option\n", str[0], str[1]);
		else
			printf("minishell: unset: '%c': invalid option\n", str[0]);
		g_master.exit_status = 2;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

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

static char	*get_env_value_cd(t_env *env, char *name)
{
	t_env	*current;

	current = env;
	if (!current || !current->name)
		return (NULL);
	while (current)
	{
		if (!ft_strcmp(current->name, name))
		{
			if (current->value)
				return (current->value);
			else
				return (NULL);
		}
		current = current->next;
	}
	return (NULL);
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

	if (argc == 1)
	{
		home_path = get_env_value_cd(g_master.env_list, "HOME");
		if (!home_path)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		return (change_directory_and_update(g_master.env_list, home_path));
	}
	if (check_option(argv[1]))
		return (EXIT_FAILURE);
	return (change_directory_and_update(g_master.env_list, argv[1]));
}
