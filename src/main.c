/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 15:32:48 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/22 14:44:51 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "minishell.h"
#include "env.h"

t_master	g_master;


int	main(void)
{
	manage_environment(&g_master.env_list);
	//print_environement_list(g_master.env_list);
	free_environment_list(g_master.env_list);
}