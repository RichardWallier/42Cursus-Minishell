# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/12 18:52:08 by rwallier          #+#    #+#              #
#    Updated: 2023/03/05 13:29:41 by rwallier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SOURCE	=	main.c command.c errorhandler.c utils.c builtin.c parse.c

OBJECTS	=	$(SOURCE:.c=.o)

LIBFTPATH		=	./libft

LIBFT			=	$(LIBFTPATH)/libft.a

MAKE	=	make -C

NAME	=	minishell

CFLAGS	= -g

LIBFILES	=	-lreadline

CC	=	gcc

RM	=	rm -f

all:	$(NAME)

$(LIBFT):
	@$(MAKE) $(LIBFTPATH)
	@$(MAKE) $(LIBFTPATH) bonus

$(NAME):	$(SOURCE) $(LIBFT)
	@$(CC) $(CFLAGS) $(SOURCE) $(LIBFT) -o $(NAME) $(LIBFILES) 
	@echo 'Minishell compiled'

clean:
	@$(RM) $(OBJECTS)
	@$(MAKE) $(LIBFTPATH) clean

fclean: clean
	@$(RM) $(NAME)
	@$(MAKE) $(LIBFTPATH) fclean

re:	fclean
	make
