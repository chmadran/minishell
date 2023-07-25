/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 14:12:20 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/25 13:32:26 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"

static int	launch_command_or_builtin(t_exec *exec, t_builtin_type type)
{
	if (type == T_ERROR || !ft_strcmp(exec->argv[0], ".")
		|| !ft_strcmp(exec->argv[0], ".."))
	{
		handle_error_cases(&g_master, exec);
		return (T_ERROR);
	}
	else if (type != T_OTHERS)
	{
		g_master.exit_status = execute_builtin(exec, type);
		return (type);
	}
	else if (!execute_command(&g_master, exec))
		return (T_ERROR);
	return (T_OTHERS);
}

static t_builtin_type	find_arg_type(char *arg)
{
	size_t					i;
	t_builtin_type			type;
	const char				*builtins[8] = {"cd", "echo", "env", "exit", "pwd",
		"unset", "export", "exit"};
	const t_builtin_type	types[8] = {T_CD, T_ECHO, T_ENV, T_EXIT, T_PWD,
		T_UNSET, T_EXPORT, T_EXIT};

	if (!arg)
		return (T_ERROR);
	i = 0;
	type = T_OTHERS;
	while (i < 8 && *builtins[i])
	{
		if (!ft_strcmp(builtins[i], arg))
		{
			type = types[i];
			break ;
		}
		i++;
	}
	return (type);
}

static void	prepare_execution(t_master *master, t_token *token, t_exec *exec)
{
	t_builtin_type	type;

	(void)exec;
	master->exec = create_arguments(token);
	launch_expansion(master->exec);
	type = find_arg_type(master->exec->argv[0]);
	print_data_builtins(master->exec);
	launch_command_or_builtin(master->exec, type);
	printf("NOW TIME TO DO THE REAL EXECUTION YEAH\n");
	return ;
}

void	launch_execution(t_master *master)
{
	t_exec		exec;
	t_token		*token;

	token = master->token_list;
	prepare_execution(master, token, &exec);
	//if (master->exit_status == 127)
	//	break ;
	//print_executable(&exec);
}
