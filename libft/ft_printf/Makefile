# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/09 16:03:25 by rwallier          #+#    #+#              #
#    Updated: 2022/06/09 16:04:22 by rwallier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

LIBFT_PATH =./libft
LIBFT =		$(LIBFT_PATH)/libft.a

SOURCES =	ft_printf.c ft_printf_utils.c ft_char_type.c ft_string_type.c ft_hex_lower_type.c
SOURCES +=	ft_hex_upper_type.c ft_int_type.c ft_unsigned_type.c ft_percent_type.c
SOURCES +=	ft_pointer_type.c

OBJECTS =	$(SOURCES:.c=.o)

NAME =		libftprintf.a

CC =		cc
CGLAGS =	-Wextra -Wall -Werror

ARFLAGS =	rc
AR =		ar
RM =		rm -f

.c.o:
			$(CC) $(CFLAGS) -c $< -o $(<:.c=.o) -I $(LIBFT_PATH)

all:		$(LIBFT) $(NAME)

$(NAME):	 $(OBJECTS)
			cp	$(LIBFT) $(NAME)
			$(AR) $(ARFLAGS) $(NAME) $(OBJECTS)

$(LIBFT):
			$(MAKE) -C $(LIBFT_PATH)
			$(MAKE) -C $(LIBFT_PATH) bonus

clean:
			$(MAKE) -C $(LIBFT_PATH) clean
			$(RM) $(OBJECTS)

fclean:		clean
			$(MAKE) -C $(LIBFT_PATH) fclean
			$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re libft
