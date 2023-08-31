/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 10:13:21 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/31 19:59:20 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"
#include "utils.h"
#include <sys/stat.h>

char	*search_pathname_command(char *command)
{
	int		i;
	char	*temp;
	char	**paths;
	t_env	*current;
	char	*pathname;

	current = g_master.env_list;
	while (current && current->name && ft_strcmp(current->name, "PATH"))
		current = current->next;
	if (!current || !current->value)
		return (NULL);
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
	struct stat	s;

	if (access(exec->argv[0], X_OK) == 0)
	{
		stat(exec->argv[0], &s);
		if (S_ISDIR(s.st_mode))
		{
			printf("minishell: %s: Is a directory\n", exec->argv[0]);
			return (g_master.exit_status = 126, T_ERROR);
		}
		exec->pathname = ft_strdup(exec->argv[0]);
	}
	else if (access(exec->argv[0], F_OK) == 0 && exec->argv[1])
	{
		printf("minishell: %s: Permission denied\n", exec->argv[0]);
		return (master->exit_status = 126, EXIT_FAILURE);
	}
	else if (ft_strncmp(exec->argv[0], ">", 1)
		&& ft_strncmp(exec->argv[0], "<", 1))
	{
		printf("minishell: %s: command not found\n", exec->argv[0]);
		return (master->exit_status = 127, T_ERROR);
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
