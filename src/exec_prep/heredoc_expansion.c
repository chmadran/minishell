/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 18:53:07 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/29 19:41:59 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"
#include "utils.h"

static char	*create_new_string_heredoc(char *substr_start, char *value,
		char *str, int i)
{
	int		j;
	char	*new_str;
	size_t	len;

	j = 0;
	if (value)
		len = ft_strlen(str) - ft_strlen(substr_start) + ft_strlen(value) + 1;
	else
		len = ft_strlen(str) - ft_strlen(substr_start) + 1;
	new_str = malloc(sizeof(char) * (len));
	while (new_str[j] && j < i)
	{
		new_str[j] = str[j];
		j++;
	}
	ft_strlcat(new_str, value, len);
	return (new_str);
}

char	*erase_name_heredoc(char *str, int i)
{
	char	*new_str;
	size_t	len;

	len = ft_strlen(str) - ft_strlen(&str[i]) + 1;
	new_str = malloc(len);
	ft_strlcpy(new_str, str, len);
	ft_strlcat(new_str, &str[i + 1], len);
	free(str);
	return (new_str);
}

static char	*process_expansion_replace_heredoc(char *substr_start, char *new_str, int i)
{
	char	*name;
	char	*value;
	char	*final_str;

	value = NULL;
	if (is_valid_name(substr_start) == EXIT_FAILURE)
		return (new_str);
	name = extract_expansion_name(substr_start);
	if (substr_start[1] == '?')
		value = ft_itoa(g_master.exit_status);
	else
		value = get_env_value(g_master.env_list, name, 1);
	if (value)
		final_str = create_new_string_heredoc(substr_start, value, new_str, i);
	else
		final_str = erase_name_heredoc(new_str, i);
	free(name);
	free(value);
	free(new_str);
	return (final_str);
}

char	*search_expansion_heredoc(char *arg)
{
	int		i;
	char	*new_str;

	i = 0;
	new_str = ft_strdup(arg);
	while (new_str[i])
	{
		if (new_str[i] == '$' && !inside_single_quotes(new_str, i))
		{
			new_str = process_expansion_replace_heredoc(&new_str[i], new_str, i);
			if (!new_str[0])
				return (NULL);
		}
		i++;
	}
	free(arg);
	arg = ft_strdup(new_str);
	return (new_str);
}