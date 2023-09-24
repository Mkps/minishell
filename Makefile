# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/24 12:58:01 by aloubier          #+#    #+#              #
#    Updated: 2023/09/22 19:13:18 by aloubier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = mshell

SRCDIR = source/

INCDIR = include/

OBJ_DIR = build

SRC = $(SRCDIR)/main.c $(SRCDIR)/signal.c

SRC_NAME = 	built-ins/unset.c \
			built-ins/built_in.c \
			built-ins/ft_cd.c \
			built-ins/dummies.c \
			cmd_list/cmd_list_find.c \
			cmd_list/cmd_list.c \
			cmd_list/cmd_list_add.c \
			cmd_list/cmd_list_utils.c \
			env/env.c \
			env/copy_env.c \
			env/copy_env_utils.c \
			env/env_utils.c \
			error.c \
			error_par.c \
			execute.c \
			export/export_utils3.c \
			export/export_utils.c \
			export/export_utils2.c \
			export/export2.c \
			export/export.c \
			free.c \
			io/io.c \
			io/here_doc.c \
			lexer.c \
			lexer_utils.c \
			main.c \
			minishell_cmd.c \
			minishell_launcher.c \
			output_error.c \
			parse_near_quote.c \
			parser.c \
			pipex_utils.c \
			prompt.c \
			signal.c \
			token.c \
			token_utils.c \
			utils/data_utils.c \
			utils/ft_split_noquote.c \
			var.c \
			var_expander.c \
			wildcards.c \
	  
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
