/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_spaces_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 10:57:40 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/31 16:26:31 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"
#include "utils.h"

int	count_new_spaces_pipe(char *data, int len)
{
	int	i;
	int	new_spaces;

	i = 0;
	new_spaces = 0;
	while (data[i])
	{
		if (ft_strncmp(&data[i], "|", 1) == 0)
		{
			if (i > 0 && data[i - 1] && ft_isalnum(data[i - 1]))
				new_spaces++;
			i++;
			if (data[i] && ft_isalnum(data[i]))
				new_spaces++;
		}
		i++;
	}
	return (new_spaces + len);
}

int	count_new_spaces(char *data, int len)
{
	int			i;
	int			add_spaces;
	const char	ops[] = "<>&";

	i = 0;
	add_spaces = 0;
	while (data[i])
	{
		if (ft_strchr(ops, data[i]))
		{
			if (i > 0 && data[i - 1] && !ft_isspace(data[i - 1])
				&& data[i - 1] != '<' && data[i - 1] != '>')
				add_spaces++;
			i++;
			if (data[i] && !ft_isspace(data[i])
				&& data[i] != '<' && data[i] != '>')
				add_spaces++;
		}
		if (!data[i])
			break ;
		i++;
	}
	return (add_spaces + len);
}
