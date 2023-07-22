/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 14:13:47 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/22 14:53:08 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/libft.h"

char	*ft_strnstr(const char *str, const char *to_find, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (to_find[j] == '\0')
		return ((char *)str);
	if (len == 0)
		return (0);
	while (str[i] && i < len)
	{
		while ((str[i + j] == to_find[j]) && str[i + j] != '\0'
			&& (i + j < len))
			j++;
		if (to_find[j] == '\0')
			return ((char *)str + i);
		i++;
		j = 0;
	}
	return (0);
}
