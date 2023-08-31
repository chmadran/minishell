/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 14:55:14 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/31 18:49:09 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "env.h"
#include "exit.h"
#include "utils.h"

void	fd_close(int fd)
{
	if (fd > 0)
		close(fd);
}

void	ft_free_main(void)
{
	free_environment_list(g_master.env_list);
	free_double_ptr(g_master.export_envp);
}

void	ft_free_child(void)
{
	free_executable();
	ft_cleanup_exit();
}

void	ft_free_lexer(void)
{
	free_token_list(g_master.token_list);
	free_double_ptr(g_master.readline_av);
	free(g_master.line_read);
}
