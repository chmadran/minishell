/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 16:30:27 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/29 15:34:04 by chmadran         ###   ########.fr       */
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

# define DEFAULT_PATH_1 "/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin"
# define DEFAULT_PATH_2 ":/opt/homebrew/bin"

# define ESTR_QUOTE "minishell: syntax error: unmatched quote\n"
# define EHDOC_PIPE "minishell: syntax error: invalid heredoc pipe\n"
# define ESTR_UNEXP "minishell: syntax error: unexpected token '%c'\n"
# define EDSTR_UNEXP "minishell: syntax error: unexpected token '%c%c'\n"
# define ESTR_OPSTART "minishell: syntax error near unexpected token 'newline'\n"

# define OP 1

// T_RED_IN,
// T_D_RED_IN,
// T_RED_OUT,
// T_D_RED_OUT,
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
	T_REDIR,
}	t_builtin_type;

typedef struct s_exec
{
	int			argc;
	char		**argv;
	char		*pathname;
	t_token		*token;
	pid_t		pid;
	int			pipefd[2];
	int			old_pipefd[2];
	bool		first_cmd;
}	t_exec;

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
	char	**export_envp;
	int		exit_status;
	int		token_count;
	int		redirection_done;

}	t_master;

/* lexer.c */
int		launch_lexer(char *line_read, t_token **token_list);
int		launch_parser(t_token **token_list);

/* lexer_utils.c */
int		is_empty_filename(t_exec *exec);
int		check_directory(char *str);
int		check_start(char *str);
int		start_operator(t_token_type type);
bool	is_in_quotes(const char *line_read, size_t j);
void	create_token_node(t_token_type type, char *data, t_token **token_list);

/* lexer_checks.c */
int		is_heredoc_pipe(t_token **token_lst);
int		is_clean(t_token **token_lst);
int		unclosed_quotes(const char *line_read);

/* print_utils.c */
int		print_export(char **envp);
void	print_data_builtins(t_exec	*current);
void	print_environement_var(t_env *env_list, char *name);
void	print_environement_list(t_env *env_list);
void	print_token_list(t_token *token_list);

/* free_utils.c */
void	free_double_ptr(char **str);
void	free_executable(void);
void	free_environment_list(t_env *env);
void	free_token_list(t_token *token_list);

/* other_utils.c */

#endif