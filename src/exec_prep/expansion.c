/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 16:42:56 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/30 12:27:14 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"
#include "utils.h"

static void	process_expansion_replace(char *substr_start, char *str,
	t_token *token, int i)
{
	char				*name;
	char				*value;
	char				*new_str;
	t_string			*s_elt;

	s_elt = malloc(sizeof(t_string));
	name = extract_expansion_name(substr_start);
	s_elt = fill_string(s_elt, name, substr_start, str);
	value = NULL;
	if (substr_start[1] == '?')
		value = ft_itoa(g_master.exit_status);
	else
		value = get_env_value(g_master.env_list, name, 1);
	if (value)
	{
		new_str = create_new_string(s_elt, value, token);
		free(token->data);
		token->data = new_str;
	}
	else if (i == 0)
		erase_token_data(token, name);
	else
		replace_name(token, name, i);
	free_string(s_elt, name, value);
}

int	launch_replace(char *arg, int i, t_token *token)
{
	if (is_valid_name(&arg[i]) == EXIT_SUCCESS)
	{
		process_expansion_replace(&arg[i], arg, token, i);
		return (EXIT_SUCCESS);
	}
	else
		return (EXIT_FAILURE);
}

void	search_expansion(t_token *token)
{
	int	i;

	i = 0;
	while (token->data[i])
	{
		if (token->data[i] == '$' && !inside_single_quotes(token->data, i) && token->data[i + 1])
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
