/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 10:13:21 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/25 13:33:11 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"

void	handle_error_cases(t_master *master, t_exec *exec)
{
	if (ft_strcmp(exec->argv[0], ".") == 0 && exec->argc == 1)
	{
		printf("minishell: .: filename argument required\n");
		printf(".: usage: . filename [arguments]\n");
		master->exit_status = 2;
	}
	else if (ft_strcmp(exec->argv[0], "..") == 0)
	{
		printf("minishell: %s: command not found\n", exec->argv[0]);
		master->exit_status = 127;
	}
	else
	{
		printf("minishell: %s: command not found\n", exec->argv[0]);
		master->exit_status = 127;
	}
}

static char	*search_path_command(t_env *env_list, char *command)
{
	int		i;
	char	*temp;
	char	**paths;
	t_env	*current;
	char	*pathname;

	current = env_list;
	while (current && current->name && ft_strcmp(current->name, "PATH"))
		current = current->next;
	if (!current || !current->value)
		paths = ft_split(DEFAULT_PATH_1 DEFAULT_PATH_2, ':');
	else
		paths = ft_split(current->value, ':');
	i = -1;
	while (paths[++i])
	{
		temp = ft_strjoin("/", command);
		pathname = ft_strjoin(paths[i], temp);
		free(temp);
		if (!access(pathname, X_OK))
			return (free_double_ptr(paths), pathname);
		free(pathname);
	}
	free_double_ptr(paths);
	return (NULL);
}

static bool	is_directory(const char *path)
{
	DIR	*dir;

	dir = opendir(path);
	if (dir)
	{
		closedir(dir);
		return (true);
	}
	return (false);
}

int	execute_command(t_master *master, t_exec *exec)
{
	exec->pathname = search_path_command(master->env_list, exec->argv[0]);
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
		else
		{
			printf("minishell: %s: command not found\n", exec->argv[0]);
			master->exit_status = 127;
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
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
		return (ft_pwd(), T_PWD);
	else if (type == T_UNSET)
		return (ft_unset(exec->argc, exec->argv), T_UNSET);
	else if (type == T_EXIT)
		ft_exit();
	return (T_ERROR);
}
