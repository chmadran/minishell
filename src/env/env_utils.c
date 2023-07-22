/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 16:39:18 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/22 14:43:40 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "env.h"
#include "libft.h"
#include "exit.h"

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

char	*update_shlvl(char *value, char *name)
{
	int	new_value;

	new_value = ft_atoi(value);
	free(value);
	new_value++;
	value = ft_itoa(new_value);
	if (!value)
	{
		free(name);
		ft_error_exit("ft_itoa (update_shlvl)", ENOMEM);
	}
	return (value);
}