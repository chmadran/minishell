/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_mem.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:24:34 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/31 14:24:29 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"
#include "utils.h"

int	erase_token_data(t_token *token, char *name, bool heredoc_limiter)
{
	int		j;
	int		k;
	char	*new_arg;

	j = 0;
	k = 0;
	if (heredoc_limiter == true)
		return (EXIT_FAILURE);
	new_arg = malloc(sizeof(char *) * (ft_strlen(token->data)
				- ft_strlen(name) + 1));
	while (token->data[j] && token->data[j] != ' ')
		j++;
	if (!token->data[j])
	{
		free(token->data);
		token->data = NULL;
		return (free(new_arg), EXIT_SUCCESS);
	}
	j++;
	while (token->data[j])
		new_arg[k++] = token->data[j++];
	new_arg[k] = '\0';
	free(token->data);
	token->data = new_arg;
	return (EXIT_SUCCESS);
}

int	replace_name(t_token *token, char *name, int i, bool heredoc_limiter)
{
	int		j;
	int		k;
	char	*new_arg;

	j = 0;
	k = 0;
	if (heredoc_limiter == true)
		return (EXIT_FAILURE);
	new_arg = malloc(sizeof(char *) * (ft_strlen(token->data)
				- ft_strlen(name) + 1));
	if (i > 0)
	{
		while (j < i)
			new_arg[k++] = token->data[j++];
		j = j + ft_strlen(name) + 1;
		while (token->data[j])
			new_arg[k++] = token->data[j++];
		new_arg[k] = '\0';
		free(token->data);
		token->data = new_arg;
	}
	return (EXIT_SUCCESS);
}

t_string	*fill_string(t_string *s_elt, char *name, char *sb_start, char *str)
{
	s_elt->substr_start = ft_strdup(sb_start);
	s_elt->name = ft_strdup(name);
	s_elt->str = ft_strdup(str);
	return (s_elt);
}

void	free_string(t_string *s_elt, char *name, char *value)
{
	free(s_elt->substr_start);
	free(s_elt->name);
	free(s_elt->str);
	free(s_elt);
	free(name);
	free(value);
}
