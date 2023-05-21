/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 17:40:24 by rwallier          #+#    #+#             */
/*   Updated: 2022/06/10 17:44:28 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include <stdio.h>

void	ft_putnbr_hexadecimal_lowercase_fd(unsigned long int n, int fd)
{
	char	character;
	char	hexadecimal_digits[17];

	ft_strlcpy(hexadecimal_digits, "0123456789abcdef", 17);
	character = '\0';
	if (n < 16)
	{
		character = hexadecimal_digits[n];
		ft_putchar_fd(character, fd);
		return ;
	}
	else
		ft_putnbr_hexadecimal_lowercase_fd(n / 16, fd);
	ft_putnbr_hexadecimal_lowercase_fd(n % 16, fd);
}

void	ft_putnbr_hexadecimal_uppercase_fd(unsigned long int n, int fd)
{
	char	character;
	char	hexadecimal_digits[17];

	ft_strlcpy(hexadecimal_digits, "0123456789ABCDEF", 17);
	character = '\0';
	if (n < 16)
	{
		character = hexadecimal_digits[n];
		ft_putchar_fd(character, fd);
		return ;
	}
	else
		ft_putnbr_hexadecimal_uppercase_fd(n / 16, fd);
	ft_putnbr_hexadecimal_uppercase_fd(n % 16, fd);
}

int	count_decimal(long long number)
{
	int	index;

	index = 0;
	if (!number)
		return (1);
	while (number)
	{
		index++;
		number /= 10;
	}
	return (index);
}

int	count_hexa(long long unsigned number)
{
	int	index;

	index = 0;
	if (!number)
		return (1);
	while (number != 0)
	{
		index++;
		number /= 16;
	}
	return (index);
}
