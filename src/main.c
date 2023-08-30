/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 15:32:48 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/30 18:39:57 by chmadran         ###   ########.fr       */
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
		g_master.exit_status = 130;
		write(2, "\n", 1);
	}
	else if (signal == SIGQUIT)
	{
		(void)signal;
		g_master.exit_status = 131;
		write(2, "Quit (core dumped)\n", 19);
	}
}

void	handle_sigint(int signal)
{
	if (signal == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 1);
		rl_redisplay();
		g_master.exit_status = 130;
	}
}

static void	manage_signals(void)
{
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
