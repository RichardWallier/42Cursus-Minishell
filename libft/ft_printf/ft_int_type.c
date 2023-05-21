/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_int_type.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 09:04:02 by rwallier          #+#    #+#             */
/*   Updated: 2022/06/10 18:23:50 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	integer_output(va_list ap)
{
	int	place_holder;

	place_holder = (va_arg(ap, int));
	ft_putnbr_fd(place_holder, 1);
	if (place_holder < 0)
		return (count_decimal(place_holder) + 1);
	return (count_decimal(place_holder));
}
