/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 16:02:09 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/21 11:25:10 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"

int	is_valid_name(char *str)
{
	if (!ft_isalpha(str[1]) && !ft_isdigit(str[1]) && str[1] != '?')
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static size_t	calculate_len(char *value)
{
	size_t	i;
	size_t	j;
	i = -1;
	while (g_master.exec->argv[++i])
	{
		j = -1;
		while (g_master.exec->argv[i][++j])
		{
			if (g_master.exec->argv[i][j] == '$')
			{
				if (value)
					return (ft_strlen(g_master.exec->argv[i])
						+ ft_strlen(value) + 1);
				else
					return (ft_strlen(g_master.exec->argv[i]) + 1);
			}
		}
	}
	return (-1);
}

static char *allocate_new_string(size_t len, char *name)
{
	char *new_str = malloc(len);

	if (!new_str)
	{
		free(name);
		free_executable();
		ft_error_exit("malloc (allocate_new_string)", ENOMEM);
		exit(EXIT_FAILURE);
	}
	return (new_str);
}

char	*create_new_string(char *substr_start, char *name, char *value,
		char *str)
{
	char	*new_str;
	size_t	len;

	len = calculate_len(value);
	new_str = allocate_new_string(len, name);
	if (value)
	{
		ft_strlcpy(new_str, str, ft_strlen(str) - ft_strlen(substr_start) + 1);
		ft_strlcat(new_str, value, len);
		ft_strlcat(new_str, substr_start + ft_strlen(name) + 1, len);
	}
	else
	{
		ft_strlcpy(new_str, str, ft_strlen(str) - ft_strlen(substr_start) + 1);
		ft_strlcat(new_str, substr_start + ft_strlen(name) + 1, len);
	}
	return (new_str);
}