/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 17:13:19 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/22 14:54:39 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size > 0)
	{
		while (src[i] && i < (size - 1))
		{
			dest[i] = src[i];
			i++;
		}
	dest[i] = '\0';
	}
	while (src[i])
		i++;
	return (i);
}