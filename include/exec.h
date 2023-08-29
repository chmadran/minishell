/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 11:06:41 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/29 11:51:43 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"

extern t_master	g_master;

/* builtins */
int				ft_cd(int argc, char **argv);
int				ft_echo(int argc, char **argv);
int				ft_env(void);
int				ft_export(int argc, char **argv);
int				ft_pwd(char **argv);
int				ft_unset(int argc, char **argv);

/* builtins_utils */
int				check_equals(char *str);
int				check_option(char *str, int flag);
int				check_event(char *str, int flag);
int				ft_array_size(char **envp);
char			**ft_sort_array(int argc, char **argv);

/*command_builtins.c*/
char			*search_pathname_command(char *command);
char			**search_paths_commands(void);

/*exec_utils.c*/

void			wait_all_processes(t_master *master);
void			init_pids(void);

/* exec_arg.c */
void			launch_execution(t_master *master);
char			*clean_arg(char *s);
void			split_args(char *s, char **argv);
t_exec			*create_arguments(t_token *token);
t_builtin_type	find_arg_type(char *arg);

/* exec_arg_utils.c */
size_t			count_spaces(char *str);
char			*allocate_memory_for_arg(char *s);
char			*handle_quoted_argument(char *s, char **arg);
char			*handle_unquoted_argument(char *s, char **arg);

/* expansion.c & expansion_utils.c*/
int				launch_expansion(t_exec *exec);
char			*create_new_string(char *substr_start, char *name, char *value,
					char *str);
int				is_valid_name(char *str);
bool			single_quotes(const char *line_read, size_t j);

/* command_builtins.c */
int				is_directory(char *path);
void			handle_error_cases(t_master *master, t_exec *exec);
int				prepare_command(t_master *master, t_exec *exec);
int				execute_builtin(t_exec *exec, t_builtin_type type);

/* exec_process.c*/
void			child_process_execution(t_master *master, t_token *token,
					t_exec *exec, t_builtin_type type);
void			parent_process_execution(t_master *master, t_token **token,
					t_exec *exec);
void			execve_execute_command(t_exec *exec, t_env *env_list,
					t_builtin_type type);
/* heredoc.c*/
int				launch_heredoc(t_exec *exec);
/* heredoc_utils.c*/
void			add_tmp_file(t_exec *exec, int hdoc_tkn);
void			clean_args(t_exec *exec, int heredoc_index);
int				check_heredoc(char **argv, int *position);

/* redir.c*/
int				launch_redirection(t_exec *exec);
int				count_redir(t_exec *exec);
int				find_redirection(char **argv);

/* redir_utils.c*/
int				clean_argv(t_exec *exec);
int				clean_redir(t_exec *exec);
int				check_redir(char **argv);

/* g_master_utils */
char			**ft_spe_split(char const *s, char c, size_t i, size_t idx);

#endif