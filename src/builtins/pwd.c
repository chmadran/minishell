/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 12:27:38 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/22 13:47:20 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"

int	ft_pwd(char **argv)
{
	char	*cwd;

	if (argv[1] && check_option(argv[1], 2))
		return (EXIT_FAILURE);
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ft_error_exit("getcwd() error", ERANGE);
		return (EXIT_FAILURE);
	}
	printf("%s\n", cwd);
	g_master.exit_status = 0;
	free(cwd);
	return (EXIT_SUCCESS);
}
