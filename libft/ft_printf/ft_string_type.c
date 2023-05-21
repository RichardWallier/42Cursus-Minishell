/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string_type.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 09:04:02 by rwallier          #+#    #+#             */
/*   Updated: 2022/06/10 19:29:21 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	string_output(va_list ap)
{
	char	*place_holder;

	place_holder = (va_arg(ap, char *));
	if (!place_holder)
	{
		ft_putstr_fd("(null)", 1);
		return (6);
	}
	ft_putstr_fd(place_holder, 1);
	return (ft_strlen(place_holder));
}
