/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_arg_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 14:28:49 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/24 14:32:01 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"

size_t	count_spaces(char *str)
{
	size_t	count;

	count = 0;
	while (*str)
	{
		if (*str == ' ')
		{
			count++;
		}
		str++;
	}
	return (count);
}

char	*allocate_memory_for_arg(char *s)
{
	char	*arg;

	arg = malloc(ft_strlen(s) + 1);
	if (!arg)
	{
		ft_error_exit("malloc in split_args", ENOMEM);
		ft_cleanup_exit();
		exit(EXIT_FAILURE);
	}
	arg[0] = '\0';
	return (arg);
}

char	*handle_quoted_argument(char *s, char **arg)
{
	char	quote;
	char	*start;
	
	quote = *s++;
	start = s;
	while (*s)
	{
		if (*s == quote)
			if ((*(s - 1) != '\\') || (*(s - 1) == '\\' && *(s - 2) == '\\'))
				break ;
		s++;
	}
	ft_strncat(*arg, start, s - start);
	if (*s)
		s++;
	return (s);
}

char	*handle_unquoted_argument(char *s, char **arg)
{
	ft_strncat(*arg, s, 1);
	s++;
	return (s);
}
