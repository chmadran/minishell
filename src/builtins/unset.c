/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 12:30:45 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/29 12:20:07 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"

static int	check_event(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '!')
		{
			printf("minishell: export: '%s': event not found\n", &str[i]);
			g_master.exit_status = 1;
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

static int	is_valid_identifier(char *str)
{
	int	i;

	i = 0;
	if (!ft_isalpha(str[0]) && str[0] != '_')
	{
		printf("minishell: unset: '%s': not a valid identifier\n", str);
		g_master.exit_status = 1;
		return (EXIT_FAILURE);
	}
	while (str[i])
	{
		if (!ft_isalpha(str[i]) && !ft_isdigit(str[i]) && str[i] != '_')
		{
			printf("minishell: unset: '%s': not a valid identifier\n", str);
			g_master.exit_status = 1;
			return (EXIT_FAILURE);
		}
		i++;
	}
	return(EXIT_SUCCESS);
}

int	ft_unset(int argc, char **argv)
{
	int		i;
	t_env	*current;

	i = 0;
	current = g_master.env_list;
	
	(void)argc;
	while (argv[++i])
	{
		if (check_event(argv[i]) || is_valid_identifier(argv[i]))
			continue ;
		while (current)
		{
			if (ft_strcmp(argv[i], current->name) == 0)
			{
				remove_var(&g_master, current);
				break ;
			}
			current = current->next;
		}
	}
	return (EXIT_SUCCESS);
}
