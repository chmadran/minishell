/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 12:27:38 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/29 11:09:35 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"

static int	check_option(char *str)
{
	if (str[0] == '-')
	{
		if (str[1])
			printf("minishell: unset: '%c%c': invalid option\n", str[0], str[1]);
		else
			printf("minishell: unset: '%c': invalid option\n", str[0]);
		g_master.exit_status = 2;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	ft_pwd(char **argv)
{
	char	*cwd;

	if (check_option(argv[1]))
		return (EXIT_FAILURE);
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ft_error_exit("getcwd() error", ERANGE);
		return (EXIT_FAILURE);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (EXIT_SUCCESS);
}
