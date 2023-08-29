/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_arg.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 14:24:07 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/29 10:33:03 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"
#include "utils.h"

t_builtin_type	find_arg_type(char *arg)
{
	size_t					i;
	t_builtin_type			type;
	const char				*builtins[8] = {"cd", "echo", "env", "exit", "pwd",
		"unset", "export", "exit"};
	const t_builtin_type	types[8] = {T_CD, T_ECHO, T_ENV, T_EXIT, T_PWD,
		T_UNSET, T_EXPORT, T_EXIT};

	if (!arg || !arg[0])
		return (T_ERROR);
	i = 0;
	type = T_OTHERS;
	while (i < 8 && *builtins[i])
	{
		if (!ft_strcmp(builtins[i], arg))
		{
			type = types[i];
			break ;
		}
		i++;
	}
	return (type);
}

char	*clean_arg(char *s)
{
	int		i;
	int		j;
	char	*news;
	int		backslash_count;

	i = 0;
	j = 0;
	backslash_count = 0;
	while (s[i])
	{
		if (s[i] == '\\' && (s[i + 1] == '"' || s[i + 1] == '\''))
			backslash_count++;
		i++;
	}
	news = ft_calloc(1, ft_strlen(s) - backslash_count + 1);
	i = 0;
	while (s[i])
	{
		if (s[i] == '\\' && (s[i + 1] == '"' || s[i + 1] == '\''))
			i++;
		news[j++] = s[i++];
	}
	news[j] = '\0';
	free(s);
	return (news);
}

void	split_args(char *s, char **argv)
{
	char	*arg;
	int		argc;

	if (!s || !argv)
		return ;
	argc = 0;
	while (*s)
	{
		arg = allocate_memory_for_arg(s);
		while (*s && *s != ' ')
		{
			if (*s == '\"' || *s == '\'')
				s = handle_quoted_argument(s, &arg);
			else
				s = handle_unquoted_argument(s, &arg);
		}
		arg = clean_arg(arg);
		argv[argc++] = arg;
		while (*s == ' ')
			s++;
	}
	argv[argc] = NULL;
}

t_exec	*create_arguments(t_token *token)
{
	t_exec	*new;

	new = ft_calloc(1, sizeof(t_exec));
	if (!new)
	{
		ft_error_exit("ft_calloc in format_arg", ENOMEM);
		ft_cleanup_exit();
		exit(EXIT_FAILURE);
	}
	if (token && token->data)
	{
		new->argv = malloc((count_spaces(token->data) + 2) * sizeof(char *));
		if (!new->argv)
		{
			ft_error_exit("malloc in create_arguments", ENOMEM);
			ft_cleanup_exit();
			exit(EXIT_FAILURE);
		}
		split_args(token->data, new->argv);
		while (new->argv[new->argc])
			new->argc++;
	}
	return (new);
}
