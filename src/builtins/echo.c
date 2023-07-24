/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 11:03:13 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/24 11:25:31 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"

int	ft_echo(int argc, char **argv)
{
	int		i;
	bool	is_option;

	i = 0;
	is_option = false;
	if (argc > 1 && !ft_strncmp(argv[1], "-n", 2))
	{
		i = 1;
		is_option = true;
	}
	while (argv[i])
	{
		ft_putstr_fd(argv[i], 1);
		if (argv[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (!is_option)
		ft_putstr_fd("\n", 1);
	return (EXIT_SUCCESS);
}
