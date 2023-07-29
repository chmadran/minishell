/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 16:47:24 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/26 18:55:37 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"
#include "libft.h"

void	ft_cleanup_exit(void)
{
	rl_clear_history();
	free_environment_list(g_master.env_list);
	free_token_list(g_master.token_list);
	free(g_master.line_read);
}

void	ft_error_exit(char *str, int error)
{
	ft_putstr_fd(str, STDERR_FILENO);
	g_master.exit_status = error;
}

void	ft_exit(void)
{
	free_executable();
	ft_cleanup_exit();
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	exit(EXIT_SUCCESS);
}
