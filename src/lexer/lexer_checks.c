/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 09:48:24 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/29 11:57:51 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exec.h"
#include "exit.h"

int	check_directory(char *str)
{
	if (str[0] == '.')
	{
		if (!str[1])
		{
			printf("minishell: .: filename argument required\n");
			g_master.exit_status = 2;
			return(EXIT_FAILURE);
		}
		if (str[1] == '.' && !str[2])
		{
			printf("minishell: ..: command not found\n");
			g_master.exit_status = 127;
			return(EXIT_FAILURE);
		}
	}
	if (is_directory(str))
	{
		printf("minishell: %s: Is a directory\n",str);
		g_master.exit_status = 126;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	check_start(char *str)
{
	if (str[0] == ':')
	{
		g_master.exit_status = 0;
		return (EXIT_FAILURE);
	}
	else if (str[0] == '!')
	{
		g_master.exit_status = 1;
		return (EXIT_FAILURE);
	}
	else if (!ft_isalpha(str[0]) && (str[0] != '\'' && str[0] != '\"') && (str[0] != '$') && (str[0] != '.'))
	{
		if (str[1] && !ft_isalpha(str[1]))
			printf(EDSTR_UNEXP, str[0], str[1]);
		else
			printf(ESTR_UNEXP, str[0]);
		g_master.exit_status = 2;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

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
			if (current->next->next && current->next->next->type > T_COMMAND)
				type = *ops[current->next->next->type - 1];
			else
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
