/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 14:32:50 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/24 14:33:16 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/libft.h"

char	*ft_strncat(char *dst, const char *src, size_t n)
{
	char	*str;
	size_t	src_len;

	str = dst;
	dst += ft_strlen(dst);
	src_len = ft_strnlen(src, n);
	dst[src_len] = '\0';
	ft_memcpy(dst, src, src_len);
	return (str);
}
