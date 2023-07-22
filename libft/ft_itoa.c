/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 14:09:19 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/22 14:54:24 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/libft.h"
#include <stdio.h>
#include <stdlib.h>

int	ft_strlen_string(int nb)
{
	int	len;

	len = (nb < 0 || nb == 0);
	while (nb != 0)
	{
		nb /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char		*newchar;
	long		num;
	int			i;
	int			neg;

	num = (n > 0) *(long)n - (n < 0) * (long) n;
	i = ft_strlen_string(n);
	newchar = malloc(sizeof(char) * i + 1);
	if (!newchar)
		return (0);
	neg = (n < 0);
	if (neg)
		newchar[0] = '-';
	newchar[i--] = '\0';
	while (i >= neg)
	{
		newchar[i] = num % 10 + '0';
		num = num / 10;
		i--;
	}
	return (newchar);
}
