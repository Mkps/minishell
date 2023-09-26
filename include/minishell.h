/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <alex.loubiere@42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 22:49:41 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/25 15:54:38 by aloubier         ###   ########.fr       */
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

# define USAGE_MSG		"Correct use is ./minishell or ./minishell \
-c \"commands to be executed\""
# define PROG_NAME		"minishell: "
# define ERR_FORK		"minishell: error creating child process\n"
# define EXPORT_MSG		"declare -x"	
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
int		here_doc_handler(t_data *data, t_io_node *io_node);
void	exec_cmd(t_cmd *cmd, t_data *data);
char	*get_cmd(char *cmd, char **env_p);
char	**get_path(char **envv);
char	*ft_readline(char *str);
void 	ft_lstadd_back_env(t_env **lst, t_env *new_node);
void 	free_env_list(t_env **env);
void	free_var(t_data *data, t_cmd *cmd);
void	print_token(t_token **root);
void	free_token(t_data *data);
char	**ft_split_noquote(char *str, char c);
int		var_is_multiple(char *var);

/**		cmd_list.c		**/
void	build_cmd_list(t_data *data, t_token *token);
int		handle_assign(t_data *data, t_token *token, t_cmd *cmd);
char	*set_assign(t_token *token);
t_cmd	*create_cmd(t_data *data);

/**		cmd_list_add.c	**/
void	add_cmd_back(t_data *data);
void	add_empty_cmd(t_data *data);
t_token	*add_cmd(t_data *data, t_token *token);

/**		cmd_list_find.c	**/
t_token	*get_cmd_first(t_token *current_t);
t_token	*get_next_cmd(t_token *src);
t_cmd	*last_cmd(t_cmd **root);

/**		cmd_list_utils.c **/
int		is_assign(char *str);
int		is_empty_cmd(t_token *start);
int		set_pipe(t_cmd *cmd);
int		get_cmd_type(t_token *token);


/** 	signal.c		**/
void	signals_interact(void);
void	signals_no_interact(void);
void	signals_here_doc(void);

/**		lexer.c			**/
int		scan_input(t_data *data);
char	*ft_str_extract(char *str, int n);
int		ft_get_word(char *input, t_data *data);
int		ft_get_sep_type(char *str);
int		ft_is_ws(char c);
int		ft_get_token(char *input, t_data *data);
int		ft_get_token_err(char *input, t_data *data);
int		ft_get_sep(char *input, t_data *data);
int		ft_get_quote(char *input, t_data *data);
void	set_parse_status_quote(t_data *data, char *input);
void	set_parse_status_par(t_data *data, char *input);

/**		parser.c		**/
void	parse_token(t_data *data);
void	parse_near_quote(t_data *data);
char	*ft_strappend(char *s1, char *s2, int mode);
void	var_expand(t_data *data, t_cmd *cmd);

/**		token_utils.c	**/
int 	token_is_quote(t_token *token);
int 	token_is_io(t_token *token);
int 	token_is_term(t_token *token);
void	lst_del_prev(t_token **node);
void	lst_del_next(t_token **node);
void	lst_del_token(t_token **node);

/**		env_utils.c		**/
int		import_envv(t_data *data, char **envv);
char	*ft_getenv(char **env, const char *str);
void	ft_setenv(t_data *data, char *value);
char	**ft_strsdup(char	**strs);

/**		cpy_env_utils.c	**/
t_env	*ft_lstnew_two(char *key, char *value);
void	ft_lstadd_back_two(t_env **lst, t_env *new_node);
void	free_env_lst(t_env **env_lst);

/**		execute.c		**/
void	execute(t_data *data);

/**		data_utils.c	**/
int		init_data(t_data *data);
void	data_cleanup(t_data *data);

/**		minishell_launcher.c	**/ void	minishell_prompt(t_data *data);
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

/**		error_par.c		**/
int		check_par_error(t_token **root);

/**		output_error.c	**/
void	output_err(char *msg, t_token *token, int squotes);
void	output_err_cmd(char *msg, char *cmd_str);
int		output_err_ret(int return_value, char *msg, char *cmd_str);

/**		variable d environnement->liste chainee **/
void 	copy_env_to_list(t_data *data);
t_env	*ft_lstnew_two(char *key, char *value);
void 	ft_lstadd_back_two(t_env **lst, t_env *new_elem);
void 	print_env_list(t_env **env_lst);

/**		execution builtin	**/
int		execute_builtin(t_cmd *cmd, t_data *data);
int		ft_echo(t_cmd *cmd);
int		ft_cd(t_cmd *cmd, t_data *data);
int		ft_pwd(t_data *data);
int		ft_env(t_data *data);
void	ft_exit(t_data *data);

/**		minishell_cmd.c	**/
int		set_fd(t_data *data, t_cmd *cmd);
void	set_pipes(t_data *data, t_cmd *cmd);
void	close_pipes(t_cmd **root, t_cmd *cmd, t_cmd *last);
void	close_fd(t_data *data, t_cmd *cmd);
int		init_io_redir(t_data *data);

/**		var.c			**/
int		is_valid_var(char *str);
char	*get_var(t_data *data,char *str);
int		var_expander(t_data *data, char *str, t_token *token);
int		retokenize(t_data *data, char *str, t_token *token);

/**		var_utils.c		**/
char	*str_replace(char *src, int r_index, int n, char *str);
char	*str_replace_strs(char **src, int r_index, int n, char *str);

/**			export			**/
void	env_update(t_data *data);
int		set_in_env(t_data *data, char *variable);
int		set_in_export(t_data *data, char *variable);
int		execute_export(t_data *data, t_cmd *cmd);
void	print_export(t_data *data);
void	sort_export_list(t_data *data);
t_cmd	*find_export_command(t_data *data);
void	env_to_export(t_data *data);
int		ft_export(t_data *data);

/**		export_utils	**/
t_export	*ft_lstnew_export(char *key, char *value, int flag);
void 		ft_lstadd_back_export(t_export **lst, t_export *new_elem);
void 		free_export_list(t_export **export_lst);
void 		insert_sorted(t_export **sorted, t_export *new_export);
char		**ft_split2(char *s, char c);
int			ft_wordsize(char *s, char c, int pos);
void		free_tabs(char **tab);
int 		key_is_valid(char *chaine);
char 		*add_quotes(char *str) ;
int 		export_key_exists(t_export *export_root, char *key_to_check);
void 		remove_export(t_data *data, const char *key_to_remove);
int 		env_key_exists(t_env *env, char *key_to_check) ;
void 		remove_env(t_data *data, const char *key_to_remove);
void 		ft_lstadd_back_env(t_env **lst, t_env *new_elem);
t_env 		*ft_lstnew_env(char *key, char *value);
void		free_env_lst(t_env **cpy);
/***	temp	*/
void 	handle_parent_directory();
void 	handle_previous_directory(t_data *data, char **old_pwd);
void 	update_oldpwd(char **old_pwd, char *new_pwd);
void 	update_pwd_and_oldpwd(t_data *data, char *new_pwd);
void 	handle_home_directory(t_data *data, const char *dir);
/***	unset	***/
int		ft_lstsize_env(t_env **lst);
void	env_update(t_data *data);
void 	execute_unset(t_data *data, t_cmd *cmd);
void 	execute_env(t_data *data, t_cmd *cmd);
t_cmd 	*find_unset_command(t_data *data);
int		ft_unset(t_data *data);

/**		dummies.c			**/
int		ft_true(void);
int		ft_false(void);

/**		wildcard_find.c	**/
char	*find_dirpath(char *f_wc, int *flag);
int		show_hidden(char *search, char *str);
char	*find_matching(char *search, char *src, char *(*function_ptr)(char *,
			char *, int), int mode);

/**		wildcard_strutils.c */
char	*ft_strjoin_tab(char **tab, int i, char join);
void	ft_str_swap(char **s1, char **s2);
char	*sort_str(char *str);
char	*chrtostr(char c);

/**		wildcard_strutils2.c **/
char	*ft_strend(char *big, char *little, char n);
char	*str_replace_free(char *src, int r_index, int n, char *str);
int		find_length(char *str, char *src, int r_index, int n);

/**		wildcard_utils.c */
int		get_start_index(char *str, int i);
int		get_end_index(char *str, int i);
char	*str_tolower(char *str);
int		ft_strcmp_no_case(const char *s1, const char *s2);

/**		wildcard_wcutils.c	**/
char	*get_front_wc(char *str);
char	*get_back_wc(char *str);
int		wc_present(char *str);

/**		wildcards.c			**/
char	*ft_wildcard(char *str);
int		wc_present(char *str);
char	*get_wildcard(char *str);
char	*get_wc_data(char *search, char *src, int mode);

int		print_envp(t_data *data);

/**		free.c				**/
void	free_child(t_data *data);
void	free_heredoc_child(t_data *data);
void	free_subshell(t_data *data);
void	data_cleanup(t_data *data);
void	free_shell(t_data *data);
int		free_data(t_data *data);
int		free_return(int return_value, void *ptr_1, void *ptr_2, void *ptr3);
void	multi_free(void *ptr_1, void *ptr_2, void *ptr_3, void *ptr_4);

/**		prompt.c			**/
char	*set_prompt(t_data *data);
void	prompt_user(t_data *data);

/**		parse_near_quote.c	**/
void	parse_near_quote(t_data *data);

#endif
