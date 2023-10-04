/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 15:20:25 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/04 09:22:03 by uaupetit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

enum					e_token_type
{
	WSPACE = 1,
	WORD,
	VAR,
	PIPE,
	PIPE_STDERR,
	IO_INPUT,
	IO_HEREDOC,
	IO_RW,
	IO_TRUNC,
	IO_APPEND,
	TERM_END,
	TERM_SC,
	TERM_2SC,
	TERM_AND,
	TERM_2AND,
	TERM_OR,
	SQUOTE,
	DQUOTE,
	O_PAR,
	C_PAR,
	BSLASH
};
enum					e_cmd_type
{
	CMD_ASSIGN = 1,
	CMD,
	COMMENT,
	EMPTY
};

typedef struct s_token
{
	struct s_token		*next;
	struct s_token		*prev;
	char				*value;
	char				*raw_value;
	int					quote_status;
	int					token_type;
	int					near_quote;
}						t_token;

typedef struct s_cmd
{
	struct s_cmd		*next;
	struct s_cmd		*prev;
	int					pid;
	int					type;
	int					fd[2];
	int					pipe_status;
	int					*pipe_fd;
	char				*cmd;
	struct s_io_node	**io_list;
	char				**args;
	struct s_env		**assign;
	int					is_term;
	int					is_bg;

}						t_cmd;

typedef struct s_AST
{
	t_token				token;
	struct s_AST		*left;
	struct s_AST		*right;
	void				*data;
}						t_AST;
typedef struct s_io_node
{
	struct s_io_node	*next;
	char				*filename;
	int					fd;
	int					mode;
}						t_io_node;

typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
}						t_env;

typedef struct s_export
{
	char				*key;
	char				*value;
	int					flag;
	struct s_export		*next;
}						t_export;

typedef struct s_data
{
	int					pid;
	int					is_interactive;
	char				**cmd_split;
	char				**envv;
	t_env				**env_cpy;
	t_export			**env_export;
	int					parse_status;
	int					exit_status;
	int					old_fd[2];
	t_token				**token_root;
	t_cmd				**cmd_list;
	char				*user_input;
	int					flag;
	int					dir_flag;
	char				*raw_input;
}						t_data;

#endif
