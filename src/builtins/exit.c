/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 16:47:24 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/22 14:28:54 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"
#include "exit.h"
#include "libft.h"

static long long	update_exit_value(char *arg)
{
	long long	value;

	value = ft_llatoi(arg);
	if (value < 0)
		return ((value % 256 + 256) % 256);
	else if (value > 255)
		return (value % 256);
	return (value);
}

void	check_numeric_arguments(int argc, char **argv)
{
	int			i;
	long long	value;

	i = -1;
	value = 0;
	if (argv[1][0] == '-' || argv[1][0] == '+')
		i++;
	while (argv[1][++i])
	{
		if (ft_isdigit(argv[1][i]) == 0)
		{
			printf("minishell: exit: %s: numeric argument required\n", argv[1]);
			g_master.exit_status = 2;
			return ;
		}
	}
	if (argc > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		g_master.exit_status = 1;
		return ;
	}
	value = update_exit_value(argv[1]);
	g_master.exit_status = value;
	return ;
}

void	ft_cleanup_exit(void)
{
	rl_clear_history();
	free_environment_list(g_master.env_list);
	free_double_ptr(g_master.export_envp);
	free_token_list(g_master.token_list);
	free(g_master.line_read);
}

void	ft_error_exit(char *str, int error)
{
	ft_putstr_fd(str, STDERR_FILENO);
	g_master.exit_status = error;
}

void	ft_exit(int argc, char **argv)
{
	remove_tmpfile();
	if (argc > 1)
		check_numeric_arguments(argc, argv);
	free_executable();
	ft_cleanup_exit();
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	exit(g_master.exit_status);
}
