/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 16:39:18 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/22 16:34:37 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"
#include "exit.h"

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

void	print_environement_list(t_env *env_list)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		printf("%s=%s\n", current->name, current->value);
		current = current->next;
	}
}

void	print_environement_var(t_env *env_list, char *name)
{
	t_env	*current;

	current = env_list;
	while (current && ft_strcmp(current->name, name))
		current = current->next;
	if (!current)
		return ;
	printf("%s=%s\n", current->name, current->value);
}