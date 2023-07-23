/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 16:30:27 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/23 18:05:58 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdbool.h>
# include <errno.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

# define ESTR_QUOTE "minishell: syntax error: unmatched quote\n"
# define ESTR_DQUOTE "minishell: syntax error: unmatched double quote\n"
# define ESTR_UNEXP "minishell: syntax error: unexpected token '%c'\n"
# define ESTR_OPSTART "minishell: syntax error near unexpected token 'newline'\n"

# define OP 5

typedef enum e_token_type
{
	T_COMMAND,
	T_PIPE,
	T_RED_IN,
	T_D_RED_IN,
	T_RED_OUT,
	T_D_RED_OUT,
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*data;
	struct s_token	*next;
	struct s_token	*last;
}	t_token;
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
	char	*line_read;
	int		exit_status;
}	t_master;

/* lexer.c */
int		launch_lexer(char *line_read, t_token **token_list);

/* lexer_utils.c */
int		start_operator(t_token_type type);
bool	is_in_quotes(const char *line_read, size_t j);
int		unclosed_quotes(const char *line_read);
void	create_token_node(t_token_type type, char *data, t_token **token_list);

/* print_utils.c */
void	print_environement_var(t_env *env_list, char *name);
void	print_environement_list(t_env *env_list);
void	print_token_list(t_token *token_list);

/* free_utils.c */
void	free_environment_list(t_env *env);
void	free_token_list(t_token *token_list);

#endif