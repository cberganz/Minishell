# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/01/29 08:20:09 by cberganz          #+#    #+#              #
#    Updated: 2022/02/20 16:30:48 by rbicanic         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME		= minishell

NAME_BONUS	= minishell_bonus

INCLUDE		= include/

HEADER		= $(INCLUDE)minishell.h

HEADER_BONUS= $(INCLUDE)minishell_bonus.h

CC			= clang

CFLAGS		= -Wall -Wextra -Werror -g3 

SRC_FILES 	=	minishell.c			\
				prompt.c			\
				signals.c			\
				utils_print.c		\
				exit.c				\
				errors.c			\
				errors_not_handled.c

SRC_BONUS_FILES = AVENIR.c

SRC_DIR		= src/
SRC			= $(addprefix $(SRC_DIR), $(SRC_FILES))

SRC_BONUS_DIR	= src_bonus/
SRC_BONUS		= $(addprefix $(SRC_BONUS_DIR), $(SRC_BONUS_FILES))

OBJ_DIR		= objs/
OBJ_FILES	= $(SRC_FILES:.c=.o)
OBJ			= $(addprefix $(OBJ_DIR), $(OBJ_FILES))

OBJ_BONUS_DIR	= objs_bonus/
OBJ_BONUS_FILES	= $(SRC_BONUS_FILES:.c=.o)
OBJ_BONUS		= $(addprefix $(OBJ_BONUS_DIR), $(OBJ_BONUS_FILES))

all: $(NAME)

bonus: $(NAME_BONUS)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -lreadline -L.local/lib -L./libft -lft -o $(NAME)

$(NAME_BONUS): $(OBJ_BONUS)
	$(CC) $(CFLAGS) $(OBJ_BONUS) -lreadline -L.local/lib -L./libft -lft -o $(NAME_BONUS)

$(OBJ_DIR):
	mkdir -p $@
$(OBJ_BONUS_DIR):
	mkdir -p $@

$(OBJ): | $(OBJ_DIR)
$(OBJ_BONUS): | $(OBJ_BONUS_DIR)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(HEADER)
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $< -o $@

$(OBJ_BONUS_DIR)%.o: $(SRC_BONUS_DIR)%.c $(HEADER_BONUS)
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(OBJ_BONUS_DIR)

fclean: clean
	rm -f $(NAME)
	rm -f $(NAME_BONUS)

re: fclean all

.PHONY : all clean fclean re
