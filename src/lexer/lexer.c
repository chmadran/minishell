/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 16:42:19 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/21 18:29:40 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"

int	end_op(t_token **token_lst)
{
	t_token			*current;

	current = *token_lst;
	while (current)
	{
		if (current->type != T_COMMAND && !current->next)
		{
			printf("syntax error near unexpected token \n");
			g_master.exit_status = 2;
			return (EXIT_FAILURE);
		}
		current = current->next;
	}
	return (EXIT_SUCCESS);
}

static t_token_type	check_token_type(char c, const char *line_read, size_t *j)
{
	size_t				i;
	t_token_type		token_type;
	const char			*ops[OP] = {"|"};
	const t_token_type	types[OP] = {T_PIPE};

	i = 0;
	token_type = T_COMMAND;
	while (i < OP)
	{
		if (is_in_quotes(line_read, *j))
			return (T_COMMAND);
		if (c != *ops[i])
		{
			i++;
			continue ;
		}
		token_type = types[i];
		if (line_read[*j + 1] == c)
			token_type = types[i + 1];
		(*j)++;
		break ;
	}
	return (token_type);
}

static int	manage_token(char *line_read, t_token **token_lst)
{
	size_t			i;
	size_t			j;
	char			*data;
	t_token_type	type;

	i = 0;
	type = check_token_type(line_read[i], line_read, &i);
	if (start_operator(type) == EXIT_FAILURE)
		return (free(line_read), EXIT_FAILURE);
	while (line_read[i])
	{
		j = i;
		type = check_token_type(line_read[i], line_read, &i);
		while (line_read[i] && type == T_COMMAND)
			type = check_token_type(line_read[++i], line_read, &i);
		data = trim_spaces(line_read, j, i - 1);
		create_token_node(T_COMMAND, data, token_lst);
		if (type != T_COMMAND)
		{
			create_token_node(type, NULL, token_lst);
			if (line_read[i])
				i++;
		}
	}
	return (EXIT_SUCCESS);
}

static void	ft_token_count(t_token **token_lst)
{
	t_token		*current;
	int			count;

	count = 0;
	current = *token_lst;
	while (current)
	{
		count++;
		current = current->next;
	}
	g_master.token_count = count;
}

// char	*reallocate_dollar(char *str)
// {
// 	char	*new_line;
// 	int		doll_counter;
// 	int		i;

// 	i = 0;

// 	return (new_line);
// }

int	launch_lexer(char *line_read, t_token **token_list)
{
	int	len;

	// line_read = reallocate_dollar(line_read);
	len = count_new_spaces(line_read, ft_strlen(line_read));
	if ((ft_strlen(line_read) <= 2 && check_start(line_read))
		|| unclosed_quotes(line_read))
		return (EXIT_FAILURE);
	if (check_directory(line_read))
		return (EXIT_FAILURE);
	line_read = add_spaces_between_ops(line_read, len);
	g_master.line_read = line_read;
	if (manage_token(line_read, token_list))
		return (EXIT_FAILURE);
	if ((is_heredoc_pipe(token_list)) || (is_clean(token_list))
		|| end_op(token_list))
	{
		free_token_list(*token_list);
		g_master.exit_status = 2;
		return (EXIT_FAILURE);
	}
	ft_token_count(token_list);
	return (EXIT_SUCCESS);
}
