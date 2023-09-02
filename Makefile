# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aloubier <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/24 12:58:01 by aloubier          #+#    #+#              #
#    Updated: 2023/08/04 07:15:42 by aloubier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = mshell

SRCDIR = source/

INCDIR = include/

OBJ_DIR = build

SRC = $(SRCDIR)/main.c $(SRCDIR)/signal.c

SRC_NAME = child.c here_doc.c \
	  main.c pipex.c pipex_cmd.c \
	  pipex_utils.c signal.c \
	  lexer.c
SRC = $(addprefix $(SRCDIR), $(SRC_NAME))
OBJ = $(SRC:$(SRCDIR)/%.c=$(OBJ_DIR)/%.o)

CC = cc

LIBFT = libft/libft.a
LIB = $(LIBFT) -lreadline

CFLAGS = -Wall -Wextra -Werror $(LIB) -I$(INCDIR)

all: $(NAME)

$(NAME): $(OBJ)
	@echo "Compiling objects for mandatory part"
	@make -s all -C libft
	$(CC) -o $(NAME) $(OBJ) $(LIB)
	
$(OBJ_DIR)/%.o:	$(SRCDIR)/%.c
	@mkdir -p '$(@D)'
	@$(CC) -c $(CFLAGS) $< -o $@

clean:
	@echo "Cleaning object files..."
	@rm -rf $(OBJ_DIR)
	@make -s clean -C libft

fclean: clean
	@echo "Cleaning binaries..."
	@rm -f $(NAME)
	@make -s fclean -C libft

re: fclean all

.PHONY: all clean fclean re