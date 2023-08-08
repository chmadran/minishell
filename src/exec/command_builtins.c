/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 10:13:21 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/29 15:14:38 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"

static char	**search_paths_commands(void)
{
	t_env	*current;

	current = g_master.env_list;
	while (current && current->name && ft_strcmp(current->name, "PATH"))
		current = current->next;
	if (!current || !current->value)
		return (ft_split(DEFAULT_PATH_1 DEFAULT_PATH_2, ':'));
	else
		return (ft_split(current->value, ':'));
	return (NULL);
}

static char	*search_pathname_command(char *command)
{
	int		i;
	char	*temp_command;
	char	**paths;
	char	*pathname;

	i = 0;
	temp_command = ft_strjoin("/", command);
	paths = search_paths_commands();
	while (paths[i])
	{
		pathname = ft_strjoin(paths[i], temp_command);
		if (access(pathname, X_OK) == 0)
			return (free(temp_command), free_double_ptr(paths), pathname);
		free(pathname);
		i++;
	}
	return (free(temp_command), free_double_ptr(paths), NULL);
}

int	is_directory(char *path)
{
	DIR	*dir;

	dir = opendir(path);
	if (dir)
	{
		closedir(dir);
		return (1);
	}
	else
		return (0);
}

int	prepare_command(t_master *master, t_exec *exec)
{
	exec->pathname = search_pathname_command(exec->argv[0]);
	if (!exec->pathname)
	{
		if (access(exec->argv[0], X_OK) == 0)
		{
			if (is_directory(exec->argv[0]))
			{
				printf("minishell: %s: Is a directory\n", exec->argv[0]);
				master->exit_status = 126;
				return (EXIT_FAILURE);
			}
			exec->pathname = ft_strdup(exec->argv[0]);
		}

		// else if (exec->argv[0])
		// {
		// 	master->redirection_done = 0;
		// 	return (T_OTHERS);
		// }
		// else
		{
			printf("minishell: %s: command not found\n", exec->argv[0]);
			master->exit_status = 127;
			return (T_ERROR);
		}
	}
	return (T_OTHERS);
}

int	execute_builtin(t_exec *exec, t_builtin_type type)
{
	if (type == T_CD)
		return (ft_cd(exec->argc, exec->argv));
	else if (type == T_ECHO)
		return (ft_echo(exec->argc, exec->argv));
	else if (type == T_ENV)
		return (ft_env(), T_ENV);
	else if (type == T_EXPORT)
		return (ft_export(exec->argc, exec->argv), T_EXPORT);
	else if (type == T_PWD)
		return (ft_pwd(exec->argv), T_PWD);
	else if (type == T_UNSET)
		return (ft_unset(exec->argc, exec->argv), T_UNSET);
	else if (type == T_EXIT)
		ft_exit(exec->argc, exec->argv);
	return (T_ERROR);
}
