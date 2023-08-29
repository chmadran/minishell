/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 16:48:12 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/29 12:11:12 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXIT_H
# define EXIT_H

# define FILENAME_ARG "minishell: .: filename argument required\n"
# define COMMAND "minishell: ..: command not found\n"
# define COMMAND_STR "minishell: %s: command not found\n"
# define PERMISSION "minishell: %s: Permission denied\n"
# define DIRECTORY "minishell: %s: Is a directory\n"
# include "minishell.h"

extern t_master	g_master;

void	ft_exit(int argc, char **argv);
void	ft_error_exit(char *str, int error);
void	ft_cleanup_exit(void);

#endif