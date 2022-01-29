# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gasselin <gasselin@student.42quebec.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/06/22 11:49:55 by gasselin          #+#    #+#              #
#    Updated: 2022/01/28 20:58:17 by gasselin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =		picross

SRCS_DIR =	srcs
INCL_DIR =	includes
OBJ_DIR =	objs

SRCS =		main.c

CC =		gcc
CFLAGS =	-Wall -Werror -Wextra
INCS =		-Imlx_mac -Iincludes
LIBS =		-framework OpenGL -framework AppKit
OBJS =		$(SRCS:.c=.o)

SRCS_PATH = $(addprefix $(SRCS_DIR)/, $(SRCS))
OBJS_PATH =	$(addprefix $(OBJ_DIR)/, $(OBJS))

all: $(NAME)

#Compiling for Linux
ifeq ($(shell uname), Linux)
$(NAME): $(OBJ_DIR) $(OBJS_PATH)
	@make re --no-print-directory -C ./mlx_linux
	@$(CC) $(CFLAGS) -D _LINUX=yes srcs/*.c -Imlx_linux -Iincludes -Lmlx_linux -lmlx -lX11 -lm -lz -lXext -o $(NAME)

#Compiling for MacOS
else
$(NAME): $(OBJ_DIR) $(OBJS_PATH)
	@make re --no-print-directory -C ./mlx_mac
	@$(CC) $(LIBS) $(OBJS_PATH) -o $(NAME) ./mlx_mac/libmlx.a
endif

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRCS_DIR)/%.c
	@$(CC) $(CFLAGS) $(INCLS) -c $< -o $@

all: $(NAME)

clean:
	@rm -rf $(OBJS_PATH) $(OBJ_DIR)
	
fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
