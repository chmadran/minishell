/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 14:55:14 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/29 11:23:11 by chmadran         ###   ########.fr       */
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
