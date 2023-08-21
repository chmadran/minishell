/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_spaces.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 16:47:52 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/21 18:16:05 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"

char	*trim_spaces(const char *str, size_t start, size_t end)
{
	size_t		i;
	size_t		length;
	char		*trimmed_str;
	const char	ops[4] = "|&<>";

	if (start >= ft_strlen(str) || end >= ft_strlen(str))
		return (NULL);
	i = 0;
	while (i < 4 && ops[i])
	{
		if (ops[i] == str[end])
			end--;
		i++;
	}
	while (ft_isspace(str[start]) && start <= end)
		start++;
	while (ft_isspace(str[end]) && end >= start)
		end--;
	length = end - start + 1;
	trimmed_str = malloc((length + 1) * sizeof(char));
	if (!trimmed_str)
		ft_error_exit("malloc error in trim_spaces", ENOMEM);
	ft_strlcpy(trimmed_str, &str[start], length + 1);
	return (trimmed_str);
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
			if (i > 0 && ft_isalnum(data[i - 1]))
				add_spaces++;
			i++;
			if (data[i] && ft_isalnum(data[i]))
				add_spaces++;
		}
		i++;
	}
	return (add_spaces + len);
}

char	*add_spaces_between_ops(char *data, int len)
{
	const char	ops[] = "<>&";
	int			i;
	int			j;
	char		*new_data;

	i = 0;
	j = 0;
	new_data = (char *)malloc(sizeof(char) * len + 1);
	if (!new_data)
		return (NULL);
	while (data[i])
	{
		if (ft_strchr(ops, data[i]))
		{
			if (i > 0 && data[i -1] && ft_isalnum(data[i - 1]))
				new_data[j++] = ' ';
			new_data[j++] = data[i++];
			if (data[i] && ft_isalnum(data[i]))
				new_data[j++] = ' ';
		}
		else
			new_data[j++] = data[i++];
	}
	new_data[j] = '\0';
	return (free(g_master.line_read), new_data);
}
