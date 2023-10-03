# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/24 12:58:01 by aloubier          #+#    #+#              #
#    Updated: 2023/10/03 16:08:07 by aloubier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCDIR = ./source

INCDIR = ./include/

OBJ_DIR = ./build

SRC_NAME =	built-ins/unset.c \
			built-ins/unset_utils.c \
			built-ins/dummies.c \
			built-ins/built_in.c \
			built-ins/ft_cd.c \
			built-ins/ft_cd_chdir.c \
			built-ins/ft_cd_utils.c \
			built-ins/ft_exit.c \
			cmd_list/cmd_list_init.c \
			cmd_list/cmd_list_assign.c \
			cmd_list/cmd_list_find.c \
			cmd_list/cmd_list_add.c \
			cmd_list/cmd_list_utils.c \
			cmd_list/cmd_list.c \
			data/free_shell.c \
			data/data_utils.c \
			data/free_cmd.c \
			data/prompt.c \
			data/free_utils.c \
			data/free.c \
			env/copy_env.c \
			env/var.c \
			env/var_utils.c \
			env/var_expander.c \
			env/env.c \
			env/env_utils.c \
			env/copy_env_utils.c \
			error/output_error.c \
			error/error.c \
			error/error_par.c \
			error/check_error.c \
			execute/execute.c \
			execute/execute_std.c \
			execute/execute_error.c \
			execute/exec_cmd.c \
			execute/execute_selector.c \
			execute/execute_nstd.c \
			export/export2.c \
			export/export3.c \
			export/export4.c \
			export/export_utils4.c \
			export/export_utils3.c \
			export/export_utils.c \
			export/export_utils2.c \
			export/export.c \
			io/pipe.c \
			io/open_io.c \
			io/here_doc_filename.c \
			io/redir_utils.c \
			io/here_doc.c \
			io/io.c \
			io/redir.c \
			io/here_doc_var.c \
			lexer/lexer.c \
			lexer/lexer_utils.c \
			lexer/lexer_token.c \
			lexer/lexer_word.c \
			main.c \
			shell/subshell.c \
			shell/minishell_launcher.c \
			shell/inline_shell.c \
			signals/signal.c \
			signals/signal_handlers.c \
			token/token_lst.c \
			token/token_utils2.c \
			token/token_utils.c \
			token/parser.c \
			token/parse_near_quote.c \
			token/token.c \
			utils/pipex_utils.c \
			utils/ft_split_noquote.c \
			utils/ft_split_noquote_utils.c \
			utils/strutils_find.c \
			wildcard/wildcard_strutils2.c \
			wildcard/wildcard_strutils.c \
			wildcard/wildcard_validate.c \
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
	@mkdir -p '$(@D)'
	@echo $(@D)
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
