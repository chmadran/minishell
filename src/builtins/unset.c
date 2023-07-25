/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 12:30:45 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/25 10:36:40 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"

int	ft_unset(int argc, char **argv)
{
	t_env	*current;

	current = g_master.env_list;
	if (argc != 2)
		return (EXIT_FAILURE);
	while (current)
	{
		if (ft_strcmp(argv[1], current->name) == 0)
		{
			remove_var(&g_master, current);
			break ;
		}
		current = current->next;
	}
	return (EXIT_SUCCESS);
}
