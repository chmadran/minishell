/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 16:30:27 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/31 15:37:24 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdbool.h>
# include <errno.h>
# include <dirent.h>
# include <limits.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>

# define DEFAULT_PATH_1 "/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin"
# define DEFAULT_PATH_2 ":/opt/homebrew/bin"

# define ESTR_QUOTE "minishell: syntax error: unmatched quote\n"
# define EHDOC_PIPE "minishell: syntax error: invalid heredoc pipe\n"
# define ESTR_UNEXP "minishell: syntax error: unexpected token '%c'\n"
# define EDSTR_UNEXP "minishell: syntax error: unexpected token '%c%c'\n"
# define ESTR_OPSTART "minishell: syntax error near unexpected token 'newline'\n"

# define OP 1

# define MAX_NB_CHILD_PIDS 1024

typedef enum e_token_type
{
	T_COMMAND,
	T_PIPE,
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*data;
	struct s_token	*next;
	struct s_token	*last;
}	t_token;

typedef enum e_builtin_type
{
	T_ERROR = 1,
	T_CD,
	T_ECHO,
	T_ENV,
	T_EXIT,
	T_PWD,
	T_UNSET,
	T_EXPORT,
	T_OTHERS,
}	t_builtin_type;

typedef struct s_exec
{
	int			argc;
	char		**argv;
	char		*pathname;
	t_token		*token;
}	t_exec;

typedef struct s_string
{
	char		*name;
	char		*value;
	char		*str;
}	t_string;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
	struct s_env	*last;
}	t_env;

typedef struct s_master
{
	t_env	*env_list;
	t_token	*token_list;
	t_exec	*exec;
	char	*line_read;
	char	**readline_av;
	char	**export_envp;
	int		size_export_envp;
	int		exit_status;
	int		token_count;
	int		flag_expan;
	pid_t	pid;
	pid_t	child_pid[MAX_NB_CHILD_PIDS];
	int		pipefd[2];
	int		tmp_fd;
	bool	first_cmd;
	int		count_pid;
	int		redir_fd_in;
	int		redir_fd_out;
	int		heredoc_sigint;
	int		heredoc_tmp_file_fd;
}	t_master;

/* lexer.c */
int		launch_lexer(char *line_read, t_token **token_list);
int		launch_parser(t_token **token_list);

/* lexer_spaces.c */
char	*trim_spaces(const char *str, size_t start, size_t end);
int		count_new_spaces(char *data, int len);
char	*add_spaces_between_ops(char *data, int len, const char	*ops);
char	*add_spaces_after_pipe(char *data);

/* lexer_spaces_utils.c */
int		count_new_spaces(char *data, int len);
int		count_new_spaces_pipe(char *data, int len);

/* lexer_utils.c */
int		is_empty_filename(t_exec *exec);
int		start_operator(t_token_type type);
bool	is_in_quotes(const char *line_read, size_t j);
void	create_token_node(t_token_type type, char *data, t_token **token_list);
char	check_more_than_two_op(t_token *current);

/* lexer_checks.c */
int		check_directory(char *str);
int		check_start(char *str);
int		is_heredoc_pipe(t_token **token_lst);
int		is_clean(t_token **token_lst);
int		unclosed_quotes(const char *line_read);

void	child_sigint(int signal);
void	handle_sigint(int signum);

#endif