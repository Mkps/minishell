# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/24 12:58:01 by aloubier          #+#    #+#              #
#    Updated: 2023/09/27 09:39:43 by uaupetit         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCDIR = ./source

INCDIR = ./include/

OBJ_DIR = ./build

SRC_NAME =	built-ins/unset.c \
			built-ins/dummies.c \
			built-ins/built_in.c \
			built-ins/ft_cd.c \
			built-ins/ft_cd_utils.c \
			cmd_list/cmd_list_find.c \
			cmd_list/cmd_list_add.c \
			cmd_list/cmd_list_utils.c \
			cmd_list/cmd_list.c \
			env/copy_env.c \
			env/env.c \
			env/env_utils.c \
			env/copy_env_utils.c \
			error.c \
			error_par.c \
			execute.c \
			export/export2.c \
			export/export_utils3.c \
			export/export_utils.c \
			export/export_utils2.c \
			export/export.c \
			free.c \
			io/here_doc.c \
			io/io.c \
			lexer.c \
			lexer_token.c \
			lexer_utils.c \
			lexer_word.c \
			main.c \
			minishell_cmd.c \
			minishell_launcher.c \
			output_error.c \
			parse_near_quote.c \
			parser.c \
			pipex_utils.c \
			prompt.c \
			signals/signal.c \
			signals/signal_handlers.c \
			token.c \
			token_utils.c \
			utils/ft_split_noquote.c \
			utils/data_utils.c \
			var.c \
			var_expander.c \
			var_utils.c \
			wildcard/wildcard_strutils2.c \
			wildcard/wildcard_strutils.c \
			wildcard/wildcard_utils.c \
			wildcard/wildcards.c \
			wildcard/wildcard_find.c \
			wildcard/wildcard_wcutils.c \
			wildcard/wildcard_get.c \

SRC = $(addprefix $(SRCDIR)/, $(SRC_NAME))
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC_NAME:c=o))

CC = cc

LIBFT = libft/libft.a
LIB = $(LIBFT) -lreadline

CFLAGS = -Wall -Wextra -Werror 

all: $(NAME)

$(NAME): $(OBJ)
	@echo "Compiling objects for mandatory part"
	@make -s all -C libft
	$(CC) -o $(NAME) $(OBJ) $(LIB) -I$(INCDIR)

$(OBJ_DIR)/%.o:	$(SRCDIR)/%.c
	mkdir -p '$(@D)'
	echo $(@D)
	$(CC) -c $(CFLAGS) $< -o $@

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
