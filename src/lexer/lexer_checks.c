/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 09:48:24 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/30 09:28:18 by chmadran         ###   ########.fr       */
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
			return (printf(FILENAME_ARG), g_master.exit_status = 2, 1);
		if (str[1] == '.' && !str[2])
			return (printf(COMMAND), g_master.exit_status = 127, 1);
		if (access(str, F_OK) == 0 && access(str, X_OK) == -1)
		{
			return (printf(PERMISSION, str), g_master.exit_status = 126, 1);
		}
	}
	if (is_directory(str))
	{
		if (str[0] == '/' || (str[0] == '.'))
			return (printf(DIRECTORY, str), g_master.exit_status = 126, 1);
		else
			return (printf(COMMAND_STR, str), g_master.exit_status = 127, 1);
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
	else if (!ft_isalpha(str[0]) && (str[0] != '\'' && str[0] != '\"')
		&& (str[0] != '$') && (str[0] != '.'))
	{
		if (str[1] && !ft_isalpha(str[1]) && str[1] != ' ')
			printf(EDSTR_UNEXP, str[0], str[1]);
		else if (str[0] != ' ')
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

char	check_more_than_two_op(t_token *current)
{
	int			i;
	int			count;
	const char	ops[4] = "|&<>";

	i = -1;
	count = 0;
	while (current->data[++i])
	{
		if (ft_strchr(ops, current->data[i]))
		{
			if ((i == 0) || current->data[i] == current->data[i - 1])
				count++;
			else
			{
				if (count == 1)
					return (current->data[i]);
				count = 1;
			}
			if (count > 2)
				return (current->data[i]);
		}
		else
			count = 0;
	}
	return (-1);
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
			if (!in_double_quote)
				in_single_quote = !in_single_quote;
		if (line_read[i] == '\"')
			if (!in_single_quote)
				in_double_quote = !in_double_quote;
	}
	if (in_single_quote || in_double_quote)
	{
		g_master.exit_status = 2;
		printf(ESTR_QUOTE);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
