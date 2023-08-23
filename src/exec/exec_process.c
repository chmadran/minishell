/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 16:18:49 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/23 17:03:32 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"

static size_t	get_env_list_size(t_env *env_list)
{
	size_t	size;

	size = 0;
	while (env_list)
	{
		size++;
		env_list = env_list->next;
	}
	return (size);
}

char	**env_list_to_array(t_env *env_list)
{
	size_t	i;
	char	*tmp;
	char	**array;

	if (!env_list)
		return (NULL);
	array = malloc((get_env_list_size(env_list) + 1) * sizeof(char *));
	if (!array)
		return (free_executable(), perror("malloc (env_list_to_array)"),
			NULL);
	i = 0;
	while (env_list)
	{
		tmp = ft_strjoin(env_list->name, "=");
		if (!tmp)
			perror("Failed to allocate memory for tmp");
		array[i] = ft_strjoin(tmp, env_list->value);
		free(tmp);
		if (!array[i])
			perror("Failed to allocate memory for array[i]");
		env_list = env_list->next;
		i++;
	}
	array[i] = NULL;
	return (array);
}

void	execve_execute_command(t_exec *exec, t_env *env_list,
			t_builtin_type type)
{
	char	**envp;

	envp = env_list_to_array(env_list);
	if (type == T_OTHERS)
		execve(exec->pathname, exec->argv, envp);
	else
	{
		g_master.exit_status = execute_builtin(exec, type);
		free_double_ptr(envp);
		return ;
	}
	free_double_ptr(envp);
	free_executable();
	perror("execve (execute_command)");
}

void	child_process_execution(t_master *master, t_token *token, t_exec *exec,
			t_builtin_type type)
{
	exec = master->exec;
	if (master->pid == 0)
	{
		dup2(master->tmp_fd, STDIN_FILENO);
		close(master->tmp_fd);
		if (token->next && token->next->type == T_PIPE)
		{
			close(master->pipefd[0]);
			dup2(master->pipefd[1], STDOUT_FILENO);
			close(master->pipefd[1]);
		}
		else if (!master->first_cmd)
		{
			close(master->pipefd[1]);
			dup2(master->pipefd[0], STDIN_FILENO);
			close(master->pipefd[0]);
		}
		if (launch_redirection(master->exec) > 0)
			execve_execute_command(master->exec, master->env_list, type);
		ft_free_child();
		exit(master->exit_status);
	}
}

void	parent_process_execution(t_master *master, t_token **token,
	t_exec *exec)
{
	exec = master->exec;
	if (master->pid != 0)
	{
		if ((*token)->next && (*token)->next->type == T_PIPE)
			master->first_cmd = false;
		else
			master->first_cmd = true;
		close(master->tmp_fd);
		if ((*token)->next)
			*token = (*token)->next->next;
		else
			*token = NULL;
		free_executable();
	}
}
