/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 16:42:56 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/31 09:40:38 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"
#include "utils.h"

bool	limiter_of_heredoc(char	*argv, int i)
{
	int		j;

	j = 0;
	while (i - 1 > 0 && ft_isspace(argv[i - 1]))
		i--;
	if (i >= 2 && ft_strncmp(&argv[i - 2], "<<", 2) == 0)
		return (true);
	return (false);
}

static int	process_expansion_replace(char *substr_start, char *str,
	t_token *token, int i)
{
	bool				heredoc_limiter;
	int					ret;
	t_string			*s_elt;

	ret = 0;
	s_elt = malloc(sizeof(t_string));
	s_elt = fill_string(s_elt, substr_start, str);
	s_elt->name = extract_expansion_name(substr_start);
	heredoc_limiter = limiter_of_heredoc(token->data, i);
	if (s_elt->substr_start[1] == '?')
		s_elt->value = ft_itoa(g_master.exit_status);
	else
		s_elt->value = get_env_value(g_master.env_list, s_elt->name, 1);
	if (s_elt->value)
	{
		s_elt->new_str = create_new_string(s_elt, s_elt->value, token);
		free(token->data);
		token->data = s_elt->new_str;
	}
	else if (i == 0)
		ret = erase_token_data(token, s_elt->name, heredoc_limiter);
	else
		ret = replace_name(token, s_elt->name, i, heredoc_limiter);
	free_string(s_elt);
	return (ret);
}

int	launch_replace(char *arg, int i, t_token *token)
{
	if (is_valid_name(&arg[i]) == EXIT_SUCCESS)
		return (process_expansion_replace(&arg[i], arg, token, i));
	else
		return (EXIT_FAILURE);
}

void	search_expansion(t_token *token)
{
	int	i;

	i = 0;
	while (token->data[i])
	{
		if (token->data[i] == '$'
			&& !inside_single_quotes(token->data, i) && token->data[i + 1])
		{
			if (launch_replace(token->data, i, token) == EXIT_FAILURE)
				i++;
			if (!token->data || !token->data[i])
				return ;
		}
		else
			i++;
	}
}

int	launch_expansion(void)
{
	t_token	*token;

	token = g_master.token_list;
	while (token)
	{
		if (token->type == T_COMMAND)
		{
			if (token->data)
				search_expansion(token);
			if (!token->data)
				return (g_master.exit_status = 0, EXIT_FAILURE);
		}
		token = token->next;
	}
	return (EXIT_SUCCESS);
}
