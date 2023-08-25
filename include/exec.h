/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 11:06:41 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/25 09:38:09 by chmadran         ###   ########.fr       */
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
int		ft_pwd(char **argv);
int		ft_unset(int argc, char **argv);

/* builtins_utils */
int		check_equals(char *str);
int		check_option(char *str, int flag);
int		check_event(char *str, int flag);
int		ft_array_size(char **envp);
char	**ft_sort_array(int argc, char **argv);

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

/* expansion.c & expansion_utils.c*/
int		launch_expansion(t_exec *exec);
char	*create_new_string(char *substr_start, char *name, char *value,
			char *str);
int		is_valid_name(char *str);

/* command_builtins.c */
int		is_directory(char *path);
void	handle_error_cases(t_master *master, t_exec *exec);
int		prepare_command(t_master *master, t_exec *exec);
int		execute_builtin(t_exec *exec, t_builtin_type type);

/* exec_process.c*/
void	child_process_execution(t_master *master, t_token *token,
			t_exec *exec, t_builtin_type type);
void	parent_process_execution(t_master *master, t_token **token, t_exec *exec);
void	execve_execute_command(t_exec *exec, t_env *env_list,
			t_builtin_type type);
char	**env_list_to_array(t_env *env_list);

/* redir.c*/
int		launch_redirection(t_exec *exec);
int		count_redir(t_exec *exec);
int		find_redirection(char **argv);

/* heredoc.c*/
int		launch_heredoc(t_exec *exec);
/* heredoc_utils.c*/
void	add_tmp_file(t_exec *exec, int hdoc_tkn);
void	clean_args(t_exec *exec, int heredoc_index);
int		check_heredoc(char **argv, int *position);

/* redir_utils.c*/
int		clean_argv(t_exec *exec);
int		clean_redir(t_exec *exec);
int		check_redir(char **argv);

/* g_master_utils */
char	**ft_spe_split(char const *s, char c, size_t i, size_t idx);

#endif