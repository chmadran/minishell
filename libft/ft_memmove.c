/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 16:24:44 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/22 14:54:02 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	int	i;

	if (dest == 0 && src == 0)
		return (0);
	if (dest > src)
	{
		i = (int)n - 1;
		while (i >= 0)
		{
			*((char *)(dest + i)) = *((char *)(src + i));
			i--;
		}
	}
	else
	{
		i = 0;
		while (i < (int)n)
		{
			*((char *)(dest + i)) = *((char *)(src + i));
			i++;
		}
	}
	return (dest);
}
