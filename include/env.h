/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 16:23:29 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/24 16:45:07 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "minishell.h"

# define SHLVL "SHLVL"

extern t_master	g_master;

/* env.c */

void	manage_environment(t_env **env_list);

/* env_utils.c */

char	*get_env_value(t_env *env, char *name);
void	remove_var(t_master *master, t_env *env);
void	add_back_env_var(t_env **env_list, t_env *var);
bool	is_var_already_in_env(t_env **env_list, t_env *var);
bool	is_valid_variable_name(char	*name, char *var_str);

#endif