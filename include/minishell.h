#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <sys/types.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <string.h>
# include <sys/wait.h>
# include <errno.h>
# include <readline/readline.h>
# include <signal.h>

# define USAGE_MSG	"Correct use is ./mshell or ./mshell -c \"commands to be executed\""
# define PROG_NAME	"minishell: "
# define ERR_FORK	"minishell: error creating child process\n"

enum escape_type{NONE, SQUOTE, DQUOTE, BACKSLASH};
enum token_type{WSPACE = 1, WORD, VAR, PIPE, PIPE_STDERR, IO_INPUT, IO_HEREDOC, IO_RW, IO_TRUNC , IO_APPEND, TERM_END, TERM_SC, TERM_AND, TERM_2AND, TERM_OR, OSQUOTE, ODQUOTE, BSLASH};
enum cmd_type {CMD_ASSIGN = 1, CMD};

typedef struct s_pipex {
	int		here_doc;
	int		count;
	int		fd[2];
	int		*status;
	int		nb_cmd;
	pid_t	*pid;
	int		**p_arr;	
}	t_pipex;

typedef struct s_token {
	struct s_token	*next;
	struct s_token	*prev;
	char			*value;
	int				token_type;
} t_token;

typedef struct s_cmd {
	struct s_cmd	*next;
	int		type;
	int		fd[2];
	char	*cmd;
	char	**args;
	
}	t_cmd;

typedef struct s_AST {
	t_token	token;
	struct s_AST	*left;
	struct s_AST	*right;
	void			*data;
}	t_AST;

typedef struct s_data {
	int		is_interactive;
	char	**envv;
	int		parse_status;
	int		exit_status;
	t_token	**token_root;
	t_cmd	**cmd_list;
	char	*user_input;	
	
}	t_data;
char	*ft_getenv(char **env, const char *str);
void	argc_error(int error_code);
void	error_exit(int exit_code);
int		open_fd(int mode, char *filename);
void	here_doc_handler(char *limiter, t_pipex *p);
void	exec_pipe(t_pipex *handler, t_cmd *cmd, char **envv);
void	exec_cmd(t_cmd *cmd, char **envv);
char	*get_cmd(char *cmd, char **env_p);
char	**get_path(char **envv);
void	last_child(int cmd_index, t_pipex *p, t_cmd *cmd, char **envv);
void	middle_child(int cmd_index, t_pipex *p, t_cmd *cmd, char **envv);
void	first_child(t_pipex *p, t_cmd *cmd, char **envv);
void	parent_handler(t_pipex *p);
void	free_pipex(t_pipex *p);
void	pipex_init(t_pipex *p);

/** 	signal.c	**/
void	signals_interact(void);
void	signals_no_interact(void);

/**		lexer.c		**/
int		scan_input(t_data *data);
char	*ft_str_extract(char *str, int n);
int		ft_get_word(char *input, t_data *data);
int		ft_get_sep_type(char *str);

/**		parser.c	**/
void	parse_token(t_data *data);
char	*var_expander(t_data *data, char *str);
void	build_cmd_list(t_data *data, t_token *token);

#endif
