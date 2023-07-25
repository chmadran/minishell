/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 15:32:48 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/25 16:55:40 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"

t_master	g_master;

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

int	main(void)
{
	rl_catch_signals = 0;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_sigint);
	manage_environment(&g_master.env_list);
	while (1)
	{
		g_master.token_list = NULL;
		g_master.line_read = readline("\033[32mminishell: \033[0m");
		if (!g_master.line_read)
			break ;
		if (ft_strlen(g_master.line_read))
			add_history(g_master.line_read);
		if (launch_lexer(g_master.line_read, &g_master.token_list))
			continue ;
		launch_execution(&g_master);
		//free_executable();
		free_token_list(g_master.token_list);
		free(g_master.line_read);
	}
	//print_environement_list(g_master.env_list);
	free_environment_list(g_master.env_list);
	//ft_cleanup_exit();
	return (0);
}
