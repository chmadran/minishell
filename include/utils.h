/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 10:23:47 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/31 18:50:02 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "minishell.h"

extern t_master	g_master;

/* free_utils.c */
void	free_double_ptr(char **str);
void	free_executable(void);
void	free_environment_list(t_env *env);
void	free_token_list(t_token *token_list);
void	remove_tmpfile(void);

/* other_utils.c */
void	ft_free_child(void);
void	ft_free_main(void);
void	ft_free_lexer(void);
void	child_sigint(int signal);
void	fd_close(int fd);

/* print_utils.c */
int		print_export(char **envp);
void	print_data_builtins(t_exec	*current);
void	print_environement_var(t_env *env_list, char *name);
void	print_environement_list(t_env *env_list);
void	print_token_list(t_token *token_list);

#endif