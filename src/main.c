/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 15:32:48 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/29 10:22:26 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"
#include "utils.h"

t_master	g_master;

void	child_sigint(int signal)
{
	if (signal == SIGINT)
	{
		(void)signal;
		write(1, "\n", 1);
	}
	else if (signal == SIGQUIT)
	{
		(void)signal;
		write(2, "Quit (core dumped)\n", 19);
	}
}

static void	handle_sigint(int signum)
{
	if (signum == SIGINT)
	{
		g_master.exit_status = 130;
		rl_replace_line("", 0);
		ft_putstr_fd("^C\n", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}

static void	manage_signals(void)
{
	rl_catch_signals = 0;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

int	main(void)
{
	manage_signals();
	manage_environment(&g_master.env_list);
	while (1)
	{
		g_master.token_list = NULL;
		g_master.line_read = readline("\033[32mminishell: \033[0m");
		if (!g_master.line_read)
		{
			ft_putstr_fd("exit\n", STDOUT_FILENO);
			break ;
		}
		if (ft_strlen(g_master.line_read))
		{
			add_history(g_master.line_read);
			if (launch_lexer(g_master.line_read, &g_master.token_list))
				continue ;
			launch_execution(&g_master);
			free_token_list(g_master.token_list);
			free(g_master.line_read);
			free_double_ptr(g_master.readline_av);
			remove_tmpfile();
		}
	}
	ft_free_main();
	return (0);
}
