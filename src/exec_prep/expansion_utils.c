/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 16:02:09 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/30 12:02:29 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"
#include "utils.h"

bool	inside_single_quotes(const char *line_read, size_t j)
{
	size_t	i;
	bool	inside_single_quotes;

	i = 0;
	inside_single_quotes = false;
	while (line_read[i] && i < j)
	{
		if (line_read[i] == '\'')
			inside_single_quotes = !inside_single_quotes;
		i++;
	}
	return (inside_single_quotes);
}

int	is_valid_name(char *str)
{
	if (str[1] && !ft_isalpha(str[1]) && !ft_isdigit(str[1]) && str[1] != '?')
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

size_t	calculate_len(char *value, t_token *token)
{
	if (value)
		return (ft_strlen(token->data)
			+ ft_strlen(value) + 1);
	else
		return (ft_strlen(token->data) + 1);
	return (-1);
}

char	*extract_expansion_name(char *str)
{
	size_t	i;
	char	*name;

	i = 1;
	name = NULL;
	if (str[i] == '?')
	{
		name = ft_strdup("?");
		return (name);
	}
	if (ft_isdigit(str[i]))
	{
		name = ft_strndup(&str[i], 1);
		return (name);
	}
	while (str[i] && str[i] != '$' && !ft_isspace(str[i]) && (ft_isalpha(str[i])
			|| str[i] == '_' || ft_isdigit(str[i])))
		i++;
	name = ft_strndup(str + 1, i - 1);
	return (name);
}

char	*create_new_string(t_string *s_elt, char *value, t_token *token)
{
	char	*new_str;
	size_t	len;

	len = calculate_len(value, token);
	new_str = malloc(len);
	if (value)
	{
		ft_strlcpy(new_str, s_elt->str,
			ft_strlen(s_elt->str) - ft_strlen(s_elt->substr_start) + 1);
		ft_strlcat(new_str, value, len);
		ft_strlcat(new_str,
			s_elt->substr_start + ft_strlen(s_elt->name) + 1, len);
	}
	return (new_str);
}
