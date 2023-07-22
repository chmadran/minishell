/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 16:23:29 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/22 16:35:05 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "minishell.h"

# define SHLVL "SHLVL"

extern t_master	g_master;

/* env_utils.c */
void	free_environment_list(t_env *env);
void	print_environement_list(t_env *env_list);
void	print_environement_var(t_env *env_list, char *name);

/* env.c */

void	manage_environment(t_env **env_list);

#endif