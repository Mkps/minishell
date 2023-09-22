/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 22:49:41 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/22 12:39:37 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include <sys/stat.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <dirent.h> 
# include "color.h"

# define USAGE_MSG		"Correct use is ./mshell or ./mshell -c \"commands to be executed\""
# define PROG_NAME		"minishell: "
# define ERR_FORK		"minishell: error creating child process\n"
# define SYNTAX_ERROR	2
# define CMD_ERR_FKO	127
# define CMD_ERR_XKO	126
# define NONE			0

extern int g_exit_code;
enum token_type{WSPACE = 1, WORD, VAR, PIPE, PIPE_STDERR, IO_INPUT, IO_HEREDOC, IO_RW, IO_TRUNC , IO_APPEND, TERM_END, TERM_SC, TERM_2SC,TERM_AND, TERM_2AND, TERM_OR, SQUOTE, DQUOTE, O_PAR, C_PAR, BSLASH};
enum cmd_type {CMD_ASSIGN = 1, CMD, COMMENT, EMPTY};

typedef struct s_token {
	struct s_token	*next;	struct s_token	*prev;
	char			*value;
	char			*raw_value;
	int				quote_status;
	int				token_type;
	int				near_quote;
} t_token;

typedef struct s_cmd {
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
	
}	t_cmd;

typedef struct s_AST {
	t_token	token;
	struct s_AST	*left;
	struct s_AST	*right;
	void			*data;
}	t_AST;
typedef struct s_io_node{
	struct s_io_node	*next;
	char				*filename;
	int					fd;
	int					mode;
}	t_io_node;

typedef struct s_env
{
	char	*key;
	char	*value;
	struct s_env	*next;
}	t_env;

typedef struct s_export
{
	char 	*export;
	char	*key;
	char	*value;
	int		flag;
	struct s_export	*next;
}	t_export;

typedef struct s_data {
	int			pid;
	int			is_interactive;
	char		**cmd_split;
	char		**envv;
	t_env		**env_cpy;
	t_export	**env_export;
	int			parse_status;
	int			exit_status;
	int			old_fd[2];
	t_token		**token_root;
	t_cmd		**cmd_list;
	char		*user_input;
	int			flag;
	char		*raw_input;
}	t_data;

void	argc_error(int error_code);
void	error_exit(int exit_code);
int		open_fd(int mode, char *filename);
int		here_doc_handler(t_data *data, char *limiter);
void	exec_cmd(t_cmd *cmd, t_data *data);
char	*get_cmd(char *cmd, char **env_p);
char	**get_path(char **envv);
char	*ft_readline(char *str);
void 	ft_lstadd_back_env(t_env **lst, t_env *new);
void 	free_env_list(t_env **env);
void	free_var(t_data *data, t_cmd *cmd);
void	print_token(t_token **root);
void	free_token(t_data *data);

/** 	signal.c	**/
void	signals_interact(void);
void	signals_no_interact(void);
void	signals_here_doc(void);
void	signals_here_doc_child(void);
void	here_doc_child_SIGINT(const int signum, void *ptr, void *envv);
void	redisplay_prompt(int signum, void *ptr);

/**		lexer.c		**/
int		scan_input(t_data *data);
char	*ft_str_extract(char *str, int n);
int		ft_get_word(char *input, t_data *data);
int		ft_get_sep_type(char *str);
int		ft_is_ws(char c);

/**		parser.c	**/
void	parse_token(t_data *data);
void	parse_near_quote(t_data *data);
void	build_cmd_list(t_data *data, t_token *token);
t_cmd	*last_cmd(t_cmd **root);
t_token	*get_cmd_first(t_token *current_t);
char	*ft_strappend(char *s1, char *s2, int mode);
void	var_expand(t_data *data, t_cmd *cmd);

/**		token_utils.c	**/
int 	token_is_quote(t_token *token);
int 	token_is_io(t_token *token);
int 	token_is_term(t_token *token);
void	lst_del_prev(t_token **node);
void	lst_del_next(t_token **node);

/**		env_utils.c		**/
int		import_envv(t_data *data, char **envv);
char	*ft_getenv(char **env, const char *str);
void	ft_setenv(t_data *data, char *value);

/**		execute.c		**/
void	execute(t_data *data);

/**		data_utils.c	**/
int		init_data(t_data *data);
void	data_cleanup(t_data *data);

/**		minishell_launcher.c	**/
void	minishell_prompt(t_data *data);
void	minishell_inline(t_data *data, char *user_input);
void	minishell_subshell(t_data *data, char *user_input);

/**		token.c			**/
t_token	*create_token(int type, char *value);
void	add_token_back(t_token **root, int type, char *value);
t_token *last_token(t_token **root);
t_token	*ft_new_token(t_token **root, char *value, int type);
void	insert_token_next(t_token *i_p, int type, char *value);

/**		lexer_utils.c	**/
int		ft_get_sep_type(char *str);
char	*ft_str_extract(char *str, int n);
char	*ft_str_extract_free(char *str, int n);
int		ft_escape_seq(char *str);
char	evaluate_bslash(char	*str, t_data *data);

/**		cmd_io.c		**/
int		handle_cmd_io(t_data *data, t_token *current_t, t_cmd *cmd);

/**		error.c			**/
int		check_error(t_data *data);
void	output_err(char *msg, t_token *token, int squotes);
void	output_err_cmd(char *msg, char *cmd_str);

/**		variable d environnement->liste chainee **/
void copy_env_to_list(t_data *data);
t_env	*ft_lstnew_two(char *key, char *value);
void ft_lstadd_back_two(t_env **lst, t_env *new);
void print_env_list(t_env **env_lst);

/**		execution builtin	**/
int		execute_builtin(t_cmd *cmd, t_data *data);
void    ft_echo(t_cmd *cmd);
void	ft_cd(t_cmd *cmd, t_data *data);
void    ft_pwd(t_data *data);
void	ft_env(t_data *data);

/**		minishell_cmd.c	**/
int		set_fd(t_data *data, t_cmd *cmd);
void	set_pipes(t_data *data, t_cmd *cmd);
void	close_pipes(t_cmd **root, t_cmd *cmd, t_cmd *last);
void	close_fd(t_data *data, t_cmd *cmd);
int		init_io_redir(t_data *data);

/**		var.c			**/
int		is_valid_var(char *str);
char	*str_replace(char *src, int r_index, int n, char *str);
void	ft_env(t_data *data);
char	*get_var(t_data *data,char *str);
char	*var_expander(t_data *data, char *str, t_token *token);

/**			export			**/
void    env_update(t_data *data);
void set_in_env(t_data *data, char *variable);
void set_in_export(t_data *data, char *variable);
void    execute_export(t_data *data, t_cmd *cmd);
void print_export(t_data *data);
void sort_export_list(t_data *data);
t_cmd *find_export_command(t_data *data);
void env_to_export(t_data *data);
void    ft_export(t_data *data);

//char *check_variable(const char *input);

/**		export_utils	**/
t_export *ft_lstnew_export(char *key, char *value, int flag);
void ft_lstadd_back_export(t_export **lst, t_export *new);
void free_export_list(t_export **export_lst);
void insert_sorted(t_export **sorted, t_export *new_export);
char    **ft_split2(char *s, char c);
int    ft_wordsize(char *s, char c, int pos);
void    free_tabs(char **tab);
int key_is_valid(char *chaine);
char *add_quotes(char *str) ;
int export_key_exists(t_export *export, char *key_to_check);
void remove_export(t_data *data, const char *key_to_remove);
int env_key_exists(t_env *env, char *key_to_check) ;
void remove_env(t_data *data, const char *key_to_remove);
void ft_lstadd_back_env(t_env **lst, t_env *new);
t_env *ft_lstnew_env(char *key, char *value);
void	free_env_lst(t_env **cpy);

/***	unset	***/
int	ft_lstsize_env(t_env **lst);
void    env_update(t_data *data);
void execute_unset(t_data *data, t_cmd *cmd);
void execute_env(t_data *data, t_cmd *cmd);
t_cmd *find_unset_command(t_data *data);
void    ft_unset(t_data *data);

/**		dummies.c			**/
int		ft_true(void);
int		ft_false(void);

/**		wildcards.c			**/
char	*ft_wildcard(char *str);
int		wc_present(char *str);

void print_envp(t_data *data);

/**		free.c				**/
void	free_child(t_data *data);
void	free_heredoc_child(t_data *data);
void	free_subshell(t_data *data);
void	data_cleanup(t_data *data);
void	free_shell(t_data *data);
int		free_data(t_data *data);

/**		prompt.c			**/
char	*set_prompt(t_data *data);
void	prompt_user(t_data *data);

/**		parse_near_quote.c	**/
void	parse_near_quote(t_data *data);


#endif
