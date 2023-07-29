/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 11:06:41 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/26 18:57:25 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"

extern t_master	g_master;

/* builtins */
int		ft_cd(int argc, char **argv);
int		ft_echo(int argc, char **argv);
int		ft_env(void);
int		ft_export(int argc, char **argv);
int		ft_pwd(void);
int		ft_unset(int argc, char **argv);

/* execution : arguments */
void	launch_execution(t_master *master);
char	*clean_arg(char *s);
void	split_args(char *s, char **argv);
t_exec	*create_arguments(t_token *token);

/* execution : arguments utils */
size_t	count_spaces(char *str);
char	*allocate_memory_for_arg(char *s);
char	*handle_quoted_argument(char *s, char **arg);
char	*handle_unquoted_argument(char *s, char **arg);

/* expansion.c */
void	launch_expansion(t_exec *exec);

/* command_builtins.c */
void	handle_error_cases(t_master *master, t_exec *exec);
int		prepare_command(t_master *master, t_exec *exec);
int		execute_builtin(t_exec *exec, t_builtin_type type);

/* exec_process.c*/
void	child_process_execution(t_master *master, t_token *token,
			t_exec *exec, t_builtin_type type);
void	parent_process_execution(t_token **token, t_exec *exec);
void	execve_execute_command(t_exec *exec, t_env *env_list,
			t_builtin_type type);

#endif