/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 18:53:07 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/30 09:46:57 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"
#include "utils.h"

char	*set_newstr_hdoc(char *substr_start, char *name, char *value, char *str)
{
	char	*new_str;
	size_t	len;

	len = ft_strlen(str) + ft_strlen(value) + 1;
	new_str = malloc(len);
	if (value)
	{
		ft_strlcpy(new_str, str, ft_strlen(str) - ft_strlen(substr_start) + 1);
		ft_strlcat(new_str, value, len);
		ft_strlcat(new_str, substr_start + ft_strlen(name) + 1, len);
	}
	return (new_str);
}

char	*erase_name_heredoc(char *str, char *name)
{
	int		j;
	int		k;
	char	*new_arg;

	j = 0;
	k = 0;
	new_arg = malloc(sizeof(char *) * (ft_strlen(str)
				- ft_strlen(name) + 1));
	while (str[j] && str[j] != ' ')
		j++;
	if (!str[j])
	{
		new_arg[0] = ' ';
		new_arg[1] = '\0';
		return (new_arg);
	}
	j++;
	while (str[j])
		new_arg[k++] = str[j++];
	new_arg[k] = '\0';
	return (new_arg);
}

static char	*process_expansion_replace_heredoc(char *substr_start,
	char *new_str, int i)
{
	char	*name;
	char	*value;
	char	*final_str;

	(void)i;
	value = NULL;
	if (is_valid_name(substr_start) == EXIT_FAILURE)
		return (new_str);
	name = extract_expansion_name(substr_start);
	if (substr_start[1] == '?')
		value = ft_itoa(g_master.exit_status);
	else
		value = get_env_value(g_master.env_list, name, 1);
	if (value)
		final_str = set_newstr_hdoc(substr_start, name, value, new_str);
	else
		final_str = erase_name_heredoc(new_str, name);
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
			new_str = process_expansion_replace_heredoc(&new_str[i],
					new_str, i);
			if (!new_str || !new_str[0])
				return (NULL);
		}
		i++;
	}
	if (ft_strcmp(arg, new_str) == 0)
	{
		free(new_str);
		return (arg);
	}
	free(arg);
	return (new_str);
}
