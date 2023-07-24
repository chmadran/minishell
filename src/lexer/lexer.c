/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 16:42:19 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/24 15:11:57 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"

static char	*trim_spaces(const char *str, size_t start, size_t end)
{
	size_t		i;
	size_t		length;
	char		*trimmed_str;
	const char	ops[4] = "|&<>";

	if (start >= ft_strlen(str) || end >= ft_strlen(str))
		return (NULL);
	i = 0;
	while (i < 4 && ops[i])
	{
		if (ops[i] == str[end])
			end--;
		i++;
	}
	while (ft_isspace(str[start]) && start <= end)
		start++;
	while (ft_isspace(str[end]) && end >= start)
		end--;
	length = end - start + 1;
	trimmed_str = malloc((length + 1) * sizeof(char));
	if (!trimmed_str)
		ft_error_exit("malloc error in trim_spaces", ENOMEM);
	ft_strlcpy(trimmed_str, &str[start], length + 1);
	return (trimmed_str);
}

static t_token_type	check_token_type(char c, const char *line_read, size_t *j)
{
	size_t				i;
	t_token_type		token_type;
	const char			*ops[OP] = {"|", "<", "<<", ">", ">>"};
	const t_token_type	types[OP] = {T_PIPE, T_RED_IN, T_D_RED_IN, T_RED_OUT,
		T_D_RED_OUT};

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

static int	manage_token(const char *line_read, t_token **token_lst)
{
	size_t			i;
	size_t			j;
	char			*data;
	t_token_type	type;

	i = 0;
	type = check_token_type(line_read[i], line_read, &i);
	if (start_operator(type) == EXIT_FAILURE)
		return (EXIT_FAILURE);
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

int	launch_lexer(char *line_read, t_token **token_list)
{
	if (unclosed_quotes(line_read))
		return (EXIT_FAILURE);
	if (manage_token(line_read, token_list))
		return (EXIT_FAILURE);
	if ((is_heredoc_pipe(token_list))
		|| (is_clean(token_list)))
	{
		free_token_list(*token_list);
		g_master.exit_status = 2;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
