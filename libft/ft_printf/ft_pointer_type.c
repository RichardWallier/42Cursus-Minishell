/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pointer_type.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 09:04:02 by rwallier          #+#    #+#             */
/*   Updated: 2022/06/10 18:23:53 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	pointer_output(va_list ap)
{
	unsigned long int		place_holder;

	place_holder = (va_arg(ap, unsigned long int));
	ft_putstr_fd("0x", 1);
	ft_putnbr_hexadecimal_lowercase_fd(place_holder, 1);
	if (!place_holder)
		return (3);
	return (count_hexa(place_holder) + 2);
}
