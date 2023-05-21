/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 17:55:01 by rwallier          #+#    #+#             */
/*   Updated: 2022/06/10 18:19:53 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "libft/libft.h"
# include <stdarg.h>

int		ft_printf(const char *s, ...);

int		is_type(char c);

int		choose_type(char c, va_list ap);

int		string_output(va_list ap);

int		char_output(va_list ap);

int		integer_output(va_list ap);

int		pointer_output(va_list ap);

int		unsigned_decimal_output(va_list ap);

int		hexadecimal_uppercase_output(va_list ap);

int		hexadecimal_lowercase_output(va_list ap);

int		percent_output(void);

void	ft_putnbr_hexadecimal_lowercase_fd(unsigned long int n, int fd);

void	ft_putnbr_hexadecimal_uppercase_fd(unsigned long int n, int fd);

void	ft_putpointer_hex(unsigned long int n, int fd);

int		count_decimal(long long number);

int		count_hexa(long long number);

#endif
