/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 11:06:41 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/31 14:24:41 by chmadran         ###   ########.fr       */
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
void			unset_from_export_env(char *argv);

/* builtins_utils */
int				check_equals(char *str);
int				check_option(char *str, int flag);
int				check_event(char *str, int flag);
int				ft_array_size(char **envp);
char			**ft_sort_array(int argc, char **argv);

/*command_builtins.c*/
char			*search_pathname_command(char *command);

/*exec_utils.c*/

void			wait_all_processes(void);
void			init_pids(void);
int				prep_command_or_error(t_exec *exec, t_builtin_type type);

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
int				launch_expansion(void);
char			*extract_expansion_name(char *str);
char			*create_new_string(t_string *s_elt, char *value,
					t_token *token);

bool			inside_single_quotes(const char *line_read, size_t j);
int				is_valid_name(char *str);
bool			single_quotes(const char *line_read, size_t j);

/* expansion_mem.c */

int				erase_token_data(t_token *token, char *name,
					bool heredoc_limiter);
int				replace_name(t_token *token, char *name, int i,
					bool heredoc_limiter);
void			free_string(t_string *s_elt, char *name, char *value);
t_string		*fill_string(t_string *s_elt, char *name,
					char *sb_start, char *str);
bool			limiter_of_heredoc(char *argv, int i);

/* command_builtins.c */
int				is_directory(char *path);
void			handle_error_cases(t_master *master, t_exec *exec);
int				prepare_command(t_master *master, t_exec *exec);
int				execute_builtin(t_exec *exec, t_builtin_type type);

/* exec_process.c*/
void			child_process_execution(t_master *master, t_token *token,
					t_exec *exec, t_builtin_type type);
void			parent_process_execution(t_master *master, t_token **token);
void			execve_execute_command(t_exec *exec, t_env *env_list,
					t_builtin_type type);
/* heredoc.c*/
int				launch_heredoc(t_exec *exec, t_master *master);
/* heredoc_utils.c*/
void			add_tmp_file(t_exec *exec, int hdoc_tkn);
void			clean_args(t_exec *exec, int heredoc_index);
int				check_heredoc(char **argv, int *position);
void			loop_ft_here(int tmp_file_fd, char *limiter);

/* heredoc_expansion.c*/
char			*search_expansion_heredoc(char *arg, char *limiter);
char			*launch_replace_heredoc(char *arg, int i, char *str,
					char *limiter);

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

/* lexer_utils */
int				end_op(char *line_read);
void			child_sigint_heredoc(int signal);

/* redir_utils2 */
int				find_redirection(char **argv);
int				spec_char_cnter(char *str, char c, int pos);
int				double_q_cnter(char *str, char c, int argv_char_count);
int				single_q_cntr(char *str, char c, int argv_char_count);

#endif