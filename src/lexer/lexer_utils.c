/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 17:27:12 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/26 18:15:25 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"

int	start_operator(t_token_type type)
{
	if (type == T_COMMAND)
		return (EXIT_SUCCESS);
	else if (type == T_PIPE)
		printf(ESTR_UNEXP, '|');
	else if (type != T_COMMAND)
		printf(ESTR_OPSTART);
	return (EXIT_FAILURE);
}

bool	is_in_quotes(const char *line_read, size_t j)
{
	size_t	i;
	bool	inside_single_quotes;
	bool	inside_double_quotes;

	i = 0;
	inside_double_quotes = false;
	inside_single_quotes = false;
	while (line_read[i] && i < j)
	{
		if (line_read[i] == '\'' && !inside_double_quotes)
			inside_single_quotes = !inside_single_quotes;
		else if (line_read[i] == '\"' && !inside_single_quotes)
			inside_double_quotes = !inside_double_quotes;
		i++;
	}
	return (inside_single_quotes || inside_double_quotes);
}

void	create_token_node(t_token_type type, char *data, t_token **token_list)
{
	t_token	*new_node;

	new_node = ft_calloc(1, sizeof(t_token));
	if (!new_node)
	{
		free(data);
		ft_error_exit("ft_calloc (create_token_node)", ENOMEM);
		return ;
	}
	new_node->data = data;
	new_node->type = type;
	if (*token_list == NULL)
	{
		*token_list = new_node;
		(*token_list)->last = new_node;
	}
	else
	{
		(*token_list)->last->next = new_node;
		(*token_list)->last = new_node;
	}
}
