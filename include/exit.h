/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 16:48:12 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/29 12:57:39 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXIT_H
# define EXIT_H

# include "minishell.h"

extern t_master	g_master;

void	ft_exit(int argc, char **argv);
void	ft_error_exit(char *str, int error);
void	ft_cleanup_exit(void);
void	remove_tmpfile(void);


#endif