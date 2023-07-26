/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 09:48:24 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/26 18:15:57 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"

int	is_heredoc_pipe(t_token **token_lst)
{
	t_token			*current;

	current = *token_lst;
	while (current)
	{
		if (current->type == T_PIPE && !current->next)
		{
			printf(EHDOC_PIPE);
			g_master.exit_status = 2;
			return (EXIT_FAILURE);
		}
		current = current->next;
	}
	return (EXIT_SUCCESS);
}

int	is_clean(t_token **token_lst)
{
	t_token			*current;
	char			type;
	const char		*ops[OP] = {"|", "<", "<<", ">", ">>"};

	current = *token_lst;
	while (current && current->next)
	{
		if (current->next->type == T_COMMAND && !ft_strlen(current->next->data)
			&& current->type > T_COMMAND)
		{
			type = *ops[current->type - 1];
			printf(ESTR_UNEXP, type);
			g_master.exit_status = 2;
			return (EXIT_FAILURE);
		}
		current = current->next;
	}
	return (EXIT_SUCCESS);
}

static bool	return_value(bool in_single_quote, bool in_double_quote)
{
	if (in_single_quote)
	{
		printf(ESTR_QUOTE);
		g_master.exit_status = 2;
		return (EXIT_FAILURE);
	}
	else if (in_double_quote)
	{
		printf(ESTR_DQUOTE);
		g_master.exit_status = 2;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	unclosed_quotes(const char *line_read)
{
	size_t	i;
	bool	in_single_quote;
	bool	in_double_quote;

	i = -1;
	in_single_quote = false;
	in_double_quote = false;
	while (line_read[++i])
	{
		if (line_read[i] == '\'')
		{
				if (!in_double_quote)
					in_single_quote = !in_single_quote;
		}
		else if (line_read[i] == '\"')
		{
				if (!in_single_quote)
					in_double_quote = !in_double_quote;
		}
	}
	return (return_value(in_single_quote, in_double_quote));
}
