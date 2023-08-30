/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils2.c                                      :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 11:06:41 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/29 16:28:02 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"
#include "utils.h"

int	single_q_cntr(char *str, char c, int argv_char_count)
{
	int	i;
	int	quotes_counter;
	int	readline_chr_counter;

	i = 0;
	quotes_counter = 0;
	readline_chr_counter = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			quotes_counter++;
		if (str[i] == c)
			readline_chr_counter++;
		if (readline_chr_counter == argv_char_count)
			break ;
		i++;
	}
	return (quotes_counter);
}

int	double_q_cnter(char *str, char c, int argv_char_count)
{
	int	i;
	int	quotes_counter;
	int	readline_chr_counter;

	i = 0;
	quotes_counter = 0;
	readline_chr_counter = 0;
	while (str[i])
	{
		if (str[i] == '\"')
			quotes_counter++;
		if (str[i] == c)
			readline_chr_counter++;
		if (readline_chr_counter == argv_char_count)
			break ;
		i++;
	}
	return (quotes_counter);
}

int	spec_char_cnter(char *str, char c, int pos)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (i <= pos && str[i])
	{
		if (str[i] == c)
			count++;
		i++;
	}
	return (count);
}

int	find_redirection(char **argv)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if ((argv[i][j] == '<' || argv[i][j] == '>')
				&& !(is_in_quotes(g_master.readline_av[i], j + 1)))
			{
				return (i);
			}
			j++;
		}
		i++;
	}
	return (-1);
}
