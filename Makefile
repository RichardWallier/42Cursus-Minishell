# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/12 18:52:08 by rwallier          #+#    #+#              #
#    Updated: 2023/06/25 17:14:35 by rwallier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SOURCE	=	main.c

OBJECTS	=	$(SOURCE:.c=.o)

LIBFTPATH		=	./libft

LIBFT			=	$(LIBFTPATH)/libft.a

MAKE	=	make -C

NAME	=	minishell

CFLAGS	= -g -L/opt/homebrew/opt/readline/lib -I/opt/homebrew/opt/readline/include

LIBFILES	=	-lreadline

CC	=	gcc

RM	=	rm -f

all:	$(NAME)

$(LIBFT):
	@$(MAKE) $(LIBFTPATH)
	@$(MAKE) $(LIBFTPATH) bonus

$(NAME):	$(SOURCE) $(LIBFT)
	@$(CC) $(CFLAGS) $(LIBFILES) $(SOURCE) $(LIBFT) -o $(NAME) #Mac M1
	@echo 'Minishell compiled'

clean:
	@$(RM) $(OBJECTS)
	@$(MAKE) $(LIBFTPATH) clean

fclean: clean
	@$(RM) $(NAME)
	@$(MAKE) $(LIBFTPATH) fclean

re:	fclean
	make
