/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 14:25:37 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/22 14:36:41 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "include/libft.h"

char	*ft_strndup(const char *s1, size_t n)
{
	size_t	i;
	char	*str;
	size_t	len;

	if (!s1)
		return (NULL);
	len = ft_strlen(s1);
	if (n < len)
		len = n;
	str = malloc((len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	while (s1[i] && i < len)
	{
		str[i] = s1[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}
