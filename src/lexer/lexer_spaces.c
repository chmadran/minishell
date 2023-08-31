/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_spaces.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 16:47:52 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/31 18:42:15 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"
#include "utils.h"

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

char	*add_spaces_between_ops(char *data, int len, const char *ops)
{
	int			i;
	int			j;
	char		*new_data;

	i = 0;
	j = 0;
	new_data = (char *)malloc(sizeof(char) * len + 2);
	if (!new_data)
		return (NULL);
	while (data[i])
	{
		if (ft_strchr(ops, data[i]) && !is_in_quotes(data, i))
		{
			if (i > 0 && data[i -1] && !ft_isspace(data[i - 1])
				&& data[i - 1] != '<' && data[i - 1] != '>')
				new_data[j++] = ' ';
			new_data[j++] = data[i++];
			if (data[i] && !ft_isspace(data[i])
				&& data[i] != '<' && data[i] != '>')
				new_data[j++] = ' ';
		}
		else
			new_data[j++] = data[i++];
	}
	return (new_data[j] = '\0', free(g_master.line_read), new_data);
}

char	*add_spaces_after_pipe(char *data)
{
	int		i;
	int		j;
	int		len;
	char	*new_data;

	i = 0;
	j = 0;
	len = count_new_spaces_pipe(data, ft_strlen(data));
	new_data = (char *)malloc(sizeof(char) * len + 1);
	while (data[i])
	{
		if (ft_strncmp(&data[i], "|", 1) == 0)
		{
			if (i > 0 && data[i -1] && ft_isalnum(data[i - 1]))
				new_data[j++] = ' ';
			new_data[j++] = data[i++];
			if (data[i] && ft_isalnum(data[i]) && !is_in_quotes(data, i))
				new_data[j++] = ' ';
		}
		else
			new_data[j++] = data[i++];
	}
	new_data[j] = '\0';
	return (free(g_master.line_read), new_data);
}

int	quotes_op(char *str)
{
	int	i;
	int	j;
	int	count;

	i = 0;
	j = 0;
	count = 0;
	while (ft_strncmp(&str[i], "\'", 1) == 0 || ft_strncmp(&str[i], "\"", 1) == 0)
		i++;
	j = i;
	while (str[i] && (str[i] == '>' || str[i] == '<'))
	{
		i++;
		count++;
	}
	while (ft_strncmp(&str[i], "\'", 1) == 0 || ft_strncmp(&str[i], "\"", 1) == 0)
		i++;
	if (!str[i] || count > 2)
	{
		printf(ESTR_UNEXP, str[j]);
		g_master.exit_status = 2;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	is_only_spaces(char *line_read)
{
	int	i;

	i = 0;
	while (line_read[i])
	{
		if (!ft_isspace(line_read[i]))
			return (EXIT_SUCCESS);
		i++;
	}
	return (EXIT_FAILURE);
}
