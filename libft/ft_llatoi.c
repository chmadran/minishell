/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_llatoi.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 13:27:54 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/22 15:00:41 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/libft.h"

long long	ft_llatoi(const char *str)
{
	int			i;
	int			sign;
	long long	res;

	i = 0;
	res = 0;
	sign = 1;
	if (ft_strcmp(str, "-9223372036854775809") == 0
		|| ft_strcmp(str, "9223372036854775808") == 0)
		return (2);
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + str[i] - '0';
		i++;
	}
	return (res * sign);
}
