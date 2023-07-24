/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnlen.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 14:33:48 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/24 14:34:08 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/libft.h"

size_t	ft_strnlen(const char *str, size_t maxlen)
{
	const char	*ptr = str;
	size_t		len;

	if (!maxlen)
		return (0);
	len = 0;
	while (len < maxlen && *ptr)
	{
		ptr++;
		len++;
	}
	return (len);
}
