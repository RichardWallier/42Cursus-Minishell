# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/12 18:52:08 by rwallier          #+#    #+#              #
#    Updated: 2023/06/29 10:14:59 by rwallier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SOURCE	=	main.c

SOURCE	+=	sighandle/set_sighandle.c

SOURCE	+=	utils/str_utils.c utils/general_utils.c

SOURCE	+=	parser/parser.c parser/parser_utils.c

SOURCE	+=	linked_list/lst_add_back.c linked_list/lst_clear.c
SOURCE	+=	linked_list/lst_new.c linked_list/lst_remove_empty_word.c
SOURCE	+=	linked_list/lst_remove_if.c linked_list/set_head.c

SOURCE	+=	lexer/lexer.c lexer/lexer_utils.c
SOURCE	+=	lexer/lexer_utils2.c lexer/lexer_utils3.c
SOURCE	+=	lexer/lexer_utils4.c

SOURCE	+=	executor/executor.c executor/executor_utils.c
SOURCE	+=	executor/executor_utils2.c executor/executor_utils3.c

SOURCE	+=	builtin/builtin.c builtin/cd.c 
SOURCE	+=	builtin/echo.c builtin/env.c 
SOURCE	+=	builtin/export.c builtin/pwd.c
SOURCE	+=	builtin/unset.c 

SOURCE	+=	wait/wait_cmd.c


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
