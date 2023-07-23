/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 17:53:58 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/23 18:05:28 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "env.h"

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

void	print_token_list(t_token *token_list)
{
	size_t	i;
	t_token	*current;

	i = 0;
	current = token_list;
	printf("\n\n---------------TABLEAU DE TOKENS-------------\n");
	printf("| %-5s | %-25s | %-5s |\n", "#", "Token Data", "Type");
	printf("---------------------------------------------\n");
	while (current)
	{
		printf("| %-5zu | %-25s | %-5d |\n", i, current->data, current->type);
		current = current->next;
		i++;
	}
	printf("---------------------------------------------\n\n\n");
}
