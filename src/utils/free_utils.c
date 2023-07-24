/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 17:56:44 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/24 14:49:16 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "env.h"

void	free_double_ptr(char **str)
{
	char	**ptr;

	if (!str)
		return ;
	ptr = str;
	while (*ptr)
		free(*ptr++);
	free(str);
}

void	free_executable(void)
{
	free_double_ptr(g_master.exec->argv);
	free(g_master.exec->pathname);
	free(g_master.exec);
}

void	free_environment_list(t_env *env)
{
	t_env	*current;
	t_env	*next;

	current = env;
	while (current)
	{
		next = current->next;
		free(current->name);
		free(current->value);
		free(current);
		current = next;
	}
}

void	free_token_list(t_token *token_list)
{
	t_token	*next;
	t_token	*current;

	current = token_list;
	while (current)
	{
		next = current->next;
		free(current->data);
		free(current);
		current = next;
	}
}
