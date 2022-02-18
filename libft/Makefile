# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cberganz <cberganz@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/29 14:42:18 by cberganz          #+#    #+#              #
#    Updated: 2022/01/14 01:06:18 by cberganz         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= libft.a

INCLUDE		= include/

HEADER		= ${INCLUDE}libft.h

CC		= clang

CFLAGS		= -Wall -Wextra -Werror

SRC_NAME	= ctype/ft_isalnum.c		\
		  	ctype/ft_isalpha.c	\
			ctype/ft_isascii.c	\
			ctype/ft_isdigit.c	\
			ctype/ft_isprint.c	\
			ctype/ft_itoa.c		\
			ctype/ft_tolower.c	\
			ctype/ft_toupper.c	\
			lst/ft_lstadd_back.c	\
			lst/ft_lstadd_front.c	\
			lst/ft_lstclear.c	\
			lst/ft_lstdelone.c	\
			lst/ft_lstiter.c	\
			lst/ft_lstlast.c	\
			lst/ft_lstmap.c		\
			lst/ft_lstnew.c		\
			lst/ft_lstsize.c	\
			mem/ft_bzero.c		\
			mem/ft_calloc.c		\
			mem/ft_memalloc.c	\
			mem/ft_memchr.c		\
			mem/ft_memcmp.c		\
			mem/ft_memcpy.c		\
			mem/ft_memdel.c		\
			mem/ft_memmove.c	\
			mem/ft_memset.c		\
			stdio/ft_putchar_fd.c	\
			stdio/ft_putstr_fd.c	\
			stdio/ft_putendl_fd.c	\
			stdio/ft_putnbr_fd.c	\
			stdlib/ft_abs.c		\
			stdlib/ft_atoi.c	\
			string/ft_split.c	\
			string/ft_strcat.c	\
			string/ft_strchr.c	\
			string/ft_strclr.c	\
			string/ft_strcmp.c	\
			string/ft_strcpy.c	\
			string/ft_strdel.c	\
			string/ft_strdup.c	\
			string/ft_strequ.c	\
			string/ft_striter.c	\
			string/ft_striteri.c	\
			string/ft_strjoin.c	\
			string/ft_strlcat.c	\
			string/ft_strlcpy.c	\
			string/ft_strlen.c	\
			string/ft_strmap.c	\
			string/ft_strmapi.c	\
			string/ft_strncat.c	\
			string/ft_strncmp.c	\
			string/ft_strncpy.c	\
			string/ft_strnequ.c	\
			string/ft_strnew.c	\
			string/ft_strnstr.c	\
			string/ft_strrchr.c	\
			string/ft_strstr.c	\
			string/ft_strtrim.c	\
			string/ft_substr.c	\
			string/ft_putnbr_base.c \
			gnl/get_next_line.c	\
			gnl/get_next_line_utils.c
			

SRC_DIR		= src/
SRC		= ${addprefix ${SRC_DIR}, ${SRC_NAME}}

OBJ_DIR		= objs/
OBJ_DIRS	= ${sort ${dir ${OBJ}}}
OBJ_NAME	= ${SRC_NAME:.c=.o}
OBJ		= ${addprefix ${OBJ_DIR}, ${OBJ_NAME}}

all: ${NAME}

${NAME}: ${OBJ}
	ar rcs ${NAME} ${OBJ}

${OBJ_DIRS}:
	mkdir -p $@

${OBJ}: | ${OBJ_DIRS}

${OBJ_DIR}%.o: ${SRC_DIR}%.c ${HEADER}
	${CC} ${CFLAGS} -I${INCLUDE} -c $< -o $@ -g3

clean:
	rm -rf ${OBJ_DIR}

fclean: clean
	rm -f ${NAME}

re: fclean all

.PHONY : all clean fclean re
