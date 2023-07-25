/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 14:12:20 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/24 18:20:07 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"

static void	prepare_execution(t_master *master, t_token *token, t_exec *exec)
{
	master->exec = create_arguments(token);
	launch_expansion(master->exec);
	(void)token;
	(void)exec;
}

void	launch_execution(t_master *master)
{
	t_exec		exec;
	t_token		*token;

	token = master->token_list;
	prepare_execution(master, token, &exec);
	//if (master->exit_status == 127)
	//	break ;
	//print_executable(&exec);
}
