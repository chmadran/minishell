/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_master_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 17:56:44 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/25 11:44:15 by avallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"

static char	**ft_free(char **str)
{
	char	**original_str;

	original_str = str;
	while (*str)
		free(*str++);
	free(original_str);
	return (NULL);
}

static size_t	ft_check_separator(char c, char charset)
{
	if (c == charset)
		return (1);
	return (0);
}

static size_t	ft_ct_strings(const char *s, char c)
{
	size_t	i;
	size_t	idx;

	i = 0;
	idx = 0;
	while (s[idx])
	{
		while (s[idx] && (ft_check_separator(s[idx], c)
				|| is_in_quotes(g_master.line_read, idx)))
			idx++;
		if (s[idx])
			i++;
		while (s[idx] && (!ft_check_separator(s[idx], c)
				|| is_in_quotes(g_master.line_read, idx)))
			idx++;
	}
	return (i);
}

static char	*ft_wd(const char *s, char c)
{
	size_t	i;
	size_t	len_word;
	char	*word;

	len_word = 0;
	i = 0;
	while (s[len_word] && (!ft_check_separator(s[len_word], c)
			|| is_in_quotes(s, len_word)))
		len_word++;
	word = malloc((len_word + 1) * sizeof(char));
	if (!word)
		return (NULL);
	while (i < len_word)
	{
		word[i] = s[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

char	**ft_spe_split(char const *s, char c, size_t i, size_t idx)
{
	char	**str;

	if (!s)
		return (NULL);
	str = malloc(sizeof(char *) * (ft_ct_strings(s, c) + 1));
	if (!str)
		return (NULL);
	while (s[idx])
	{
		while (s[idx] && (ft_check_separator(s[idx], c)
				|| is_in_quotes(g_master.line_read, idx)))
			idx++;
		if (s[idx])
		{
			str[i] = ft_wd(&s[idx], c);
			if (!str[i++])
				return (ft_free(str));
		}
		while (s[idx] && (!ft_check_separator(s[idx], c)
				|| is_in_quotes(g_master.line_read, idx)))
			idx++;
	}
	str[i] = NULL;
	return (str);
}
