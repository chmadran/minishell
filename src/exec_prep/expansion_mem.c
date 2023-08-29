/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_mem.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:24:34 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/29 15:25:38 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"
#include "utils.h"

void	erase_token_data(t_token *token, char *name)
{
	int		j;
	int		k;
	char	*new_arg;

	j = 0;
	k = 0;
	new_arg = malloc(sizeof(char *) * (ft_strlen(token->data)
				- ft_strlen(name) + 1));
	while (token->data[j] && token->data[j] != ' ')
		j++;
	if (!token->data[j])
	{
		free(token->data);
		free(new_arg);
		token->data = NULL;
		return ;
	}
	j++;
	while (token->data[j])
		new_arg[k++] = token->data[j++];
	new_arg[k] = '\0';
	free(token->data);
	token->data = new_arg;
}

void	replace_name(t_token *token, char *name, int i)
{
	int		j;
	int		k;
	char	*new_arg;

	j = 0;
	k = 0;
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
}
