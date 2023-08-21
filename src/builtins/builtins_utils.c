/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 14:44:01 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/21 11:42:42 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"

int	check_equals(char *str)
{
	if (str[0] == '=')
	{
		printf("minishell: export: '%s': not a valid identifier\n", str);
		g_master.exit_status = 1;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	check_option(char *str, int flag)
{
	char	*function_name;

	if (flag == 0)
		function_name = "cd";
	else if (flag == 1)
		function_name = "export";
	else if (flag == 2)
		function_name = "pwd";
	if (str[0] == '-')
	{
		if (str[1])
			printf("minishell: %s: '%c%c': invalid option\n", function_name,
				str[0], str[1]);
		else
			printf("minishell: %s: '%c': invalid option\n", function_name,
				str[0]);
		g_master.exit_status = 2;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	check_event(char *str, int flag)
{
	char	*function_name;
	int		i;

	i = 0;
	if (flag == 0)
		function_name = "export";
	else if (flag == 1)
		function_name = "unset";
	while (str[i])
	{
		if (str[i] == '!')
		{
			printf("minishell: %s: '%s': event not found\n", function_name,
				&str[i]);
			g_master.exit_status = 1;
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

int	ft_array_size(char **envp)
{
	int	size;

	size = 0;
	while (envp[size])
		size++;
	return (size);
}

char	**ft_sort_array(int argc, char **argv)
{
	char	*buf;
	int		i;

	i = 1;
	while (i < argc)
	{
		while (i < argc - 1)
		{
			if (ft_strcmp(argv[i], argv[i + 1]) > 0)
			{
				buf = argv[i + 1];
				argv[i + 1] = argv[i];
				argv[i] = buf;
			}
			i++;
		}
		i = 1;
		argc--;
	}
	return (argv);
}
