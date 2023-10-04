/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 22:49:41 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/04 14:46:58 by uaupetit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "color.h"
# include "define.h"
# include "struct.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

extern int	g_exit_code;

/**		built-ins		**/
/***		execution builtin	***/
int			execute_builtin(t_cmd *cmd, t_data *data);
int			ft_cd(t_cmd *cmd, t_data *data);
int			ft_pwd(t_data *data);
int			ft_env(t_data *data);
int			ft_exit(t_data *data, t_cmd *cmd);
int			update_pwd_and_oldpwd(t_data *data, char *pwd, char *temp);
int			set_pwd(char *pwd, char *dir);
void		env_unset_free(t_env *current);
char		*ft_getenvcpy(t_data *data, char *key);

/**		cmd_list		**/
/***		cmd_list.c		***/
void		build_cmd_list(t_data *data, t_token *token);
int			handle_assign(t_data *data, t_token *token, t_cmd *cmd);
char		*set_assign(t_token *token);
t_cmd		*create_cmd(t_data *data);
/***		cmd_list_add.c	***/
void		add_cmd_back(t_data *data);
void		add_empty_cmd(t_data *data);
t_token		*add_cmd(t_data *data, t_token *token);
char		*cmd_get_args(t_token *token);
/***		cmd_list_find.c	***/
t_token		*get_cmd_first(t_token *current_t);
t_token		*get_next_cmd(t_token *src);
t_cmd		*last_cmd(t_cmd **root);
/***		cmd_list_utils.c ***/
int			is_assign(char *str);
int			is_empty_cmd(t_token *start);
int			set_pipe(t_cmd *cmd);
int			get_cmd_type(t_token *token);
/***		cmd_list_init.c		**/
t_cmd		*init_new_cmd(t_data *data, t_token *token);
int			handle_assign(t_data *data, t_token *token, t_cmd *cmd);

/**		data			**/
void		free_cmd_list(t_data *data);
/***		data_utils.c	***/
int			init_data(t_data *data);
void		data_cleanup(t_data *data);
int			execute_builtin(t_cmd *cmd, t_data *data);
void		free_null(char **str);
/***		prompt.c			***/
char		*set_prompt(t_data *data);
void		prompt_user(t_data *data);

/**		env				**/
int			retokenize(t_data *data, char *str, t_token *token);
/***		env_utils.c		***/
int			import_envv(t_data *data, char **envv);
char		*ft_getenv(char **env, const char *str);
void		ft_setenv(t_data *data, char *value);
char		**ft_strsdup(char **strs);
/***		cpy_env_utils.c	***/
t_env		*ft_lstnew_two(char *key, char *value);
void		ft_lstadd_back_two(t_env **lst, t_env *new_node);
void		free_env_lst(t_env **env_lst);
void		free_env_node(t_env *env_node);

/**		error			**/
/***		error.c			***/
int			check_error(t_data *data);
int			check_error_raw(t_data *data);
/***		error_par.c		***/
int			check_par_error(t_token **root);
/***		output_error.c	***/
void		output_err(char *msg, t_token *token, int squotes);
void		output_err_cmd(char *msg, char *cmd_str);
int			output_err_ret(int return_value, char *msg, char *cmd_str);

/**		execute			**/
void		execute_empty(t_cmd *cmd, t_data *data);
void		execute_subshell(t_cmd *cmd, t_data *data);
int			is_standalone(t_cmd *cmd);
void		execute_parent(t_cmd *cmd, t_data *data);
void		execute_child(t_cmd *cmd, t_data *data);
int			is_unpiped(t_cmd *cmd);
int			is_builtin(t_cmd *cmd);
void		exec_cmd(t_cmd *cmd, t_data *data);
/***		execute.c		***/
void		execute(t_data *data);
int			get_cmd_ecode(t_cmd *cmd, t_data *data);
t_cmd		*conditional(t_data *data, t_cmd *current);
int			fd_valid(t_cmd *cmd);

/**		export			**/
void		export_unset_free(t_export *current);

/**		io				**/
int			open_fd_node(t_data *data, t_cmd *cmd, t_io_node *fd);
int			close_fd_set(int fdin, int fdout);
int			close_cmd_fd(t_cmd *cmd);
int			dup_close_fd_set(int fdin, int fdout);
int			set_hd(t_data *data, t_cmd *cmd);
t_token		*wc_tokenize(t_token *start, char *str, t_data *data);
int			set_hd(t_data *data, t_cmd *cmd);
/***		here_doc		***/
int			here_doc_handler(t_data *data, t_io_node *io_node);
char		*heredoc_var_expand(t_data *data, char *str, int flag);
int			get_flag(char *limiter);
char		*generate_heredoc_filename(void);
int			open_fd_node(t_data *data, t_cmd *cmd, t_io_node *fd);

/**		lexer			**/
int			scan_input(t_data *data);
char		*ft_str_extract(char *str, int n);
int			ft_get_word(char *input, t_data *data);
int			ft_get_sep_type(char *str);
int			ft_is_ws(char c);
int			ft_get_token(char *input, t_data *data);
int			ft_get_token_err(char *input, t_data *data);
int			ft_get_sep(char *input, t_data *data);
int			ft_get_quote(char *input, t_data *data);

/**		shell		**/
void		set_shlevel(t_data *data);
void		minishell_inline(t_data *data, char *user_input);
int			minishell_subshell(t_data *data, char *user_input);
void		minishell_prompt(t_data *data);
void		minishell_core(t_data *data);

/**		signals		**/
/*** 		signal_hanlers.c	***/
void		signals_interact(void);
void		signals_no_interact(void);
void		signals_here_doc(void);
void		signal_parent(void);
/***		signal.c			***/
void		redisplay_prompt(int signum);
void		signal_quit(int signum);
void		signal_nl(int signum);
void		signal_sigint_heredoc(int signum);
void		set_sigign(void);

/**		token		**/
/**		parser.c		**/
void		parse_token(t_data *data);
char		*ft_strappend(char *s1, char *s2, int mode);
void		var_expand(t_data *data, t_cmd *cmd);
void		set_parse_status_quote(t_data *data, char *input);
void		set_parse_status_par(t_data *data, char *input);
void		parse_near_quote(t_data *data);
/***		token_utils.c	***/
int			token_is_quote(t_token *token);
int			token_is_io(t_token *token);
int			token_is_term(t_token *token);
void		lst_del_prev(t_token **node);
void		lst_del_next(t_token **node);
void		lst_del_token(t_token **node);

/**		utils		**/
int			cycle_count_strings(int i, char *str, char separator);
int			get_extracted_strlen(char *src, char separator);
int			cycle_through(char *str, char c);
void		error_exit(int exit_code);
int			open_fd(int mode, char *filename);

/**		wildcard	**/
/***		wildcard_find.c	**/
char		*find_dirpath(char *f_wc, int *flag);
int			show_hidden(char *search, char *str);
char		*find_matching(char *search, char *src,
				char *(*function_ptr)(char *, char *, int), int mode);
char		*build_str(char *dirpath, char *str, int flag);
/***		wildcard_strutils.c */
char		*ft_strjoin_tab(char **tab, int i, char join);
void		ft_str_swap(char **s1, char **s2);
char		*sort_str(char *str);
char		*chrtostr(char c);
/***		wildcard_strutils2.c **/
char		*ft_strend(char *big, char *little, char n);
char		*str_replace_free(char *src, int r_index, int n, char *str);
int			find_length(char *str, char *src, int r_index, int n);
size_t		wc_minlen(char *str);
/***		wildcard_utils.c */
int			get_start_index(char *str, int i);
int			get_end_index(char *str, int i);
char		*str_tolower(char *str);
int			ft_strcmp_no_case(const char *s1, const char *s2);
/***		wildcard_validate.c	**/
char		*validate_str(char *str, char *wc);
/***		wildcard_wcutils.c	**/
char		*get_front_wc(char *str);
char		*get_back_wc(char *str);
int			wc_present(char *str);
/***		wildcards.c			**/
char		*ft_wildcard(char *str);
int			wc_present(char *str);
char		*get_wildcard(char *str);
char		*get_wc_data(char *search, char *src, int mode);

/**		token.c			**/
t_token		*create_token(int type, char *value);
void		add_token_back(t_token **root, int type, char *value);
t_token		*last_token(t_token **root);
t_token		*ft_new_token(t_token **root, char *value, int type);
void		insert_token_next(t_token *i_p, int type, char *value);

/**		lexer_utils.c	**/
int			ft_get_sep_type(char *str);
char		*ft_str_extract(char *str, int n);
char		*ft_str_extract_free(char *str, int n);
int			ft_escape_seq(char *str);
char		evaluate_bslash(char *str, t_data *data);

/**		cmd_io.c		**/
int			handle_cmd_io(t_data *data, t_token *current_t, t_cmd *cmd);

/**		error.c			**/
int			check_error(t_data *data);

/**		error_par.c		**/
int			check_par_error(t_token **root);

/**		output_error.c	**/
void		output_err(char *msg, t_token *token, int squotes);
void		output_err_cmd(char *msg, char *cmd_str);
int			output_err_ret(int return_value, char *msg, char *cmd_str);

/**		variable d environnement->liste chainee **/
void		copy_env_to_list(t_data *data);
t_env		*ft_lstnew_two(char *key, char *value);
void		ft_lstadd_back_two(t_env **lst, t_env *new_elem);
void		print_env_list(t_env **env_lst);

/**		execution builtin	**/
int			execute_builtin(t_cmd *cmd, t_data *data);
int			ft_echo(t_cmd *cmd, int i);
int			ft_cd(t_cmd *cmd, t_data *data);
int			ft_cd_next(char *pwd, char *dir, t_data *data, char *old_pwd);
int			ft_pwd(t_data *data);
int			ft_env(t_data *data);
int			ft_exit(t_data *data, t_cmd *cmd);
int			handle_regular_directory(char *dir, t_data *data);
int			update_pwd_and_oldpwd(t_data *data, char *pwd, char *temp);
int			handle_directory_change(t_data *data, char **old_pwd, char *dir);
void		env_unset_free(t_env *current);
void		export_unset_free(t_export *current);

/**		minishell_cmd.c	**/
int			set_fd(t_data *data, t_cmd *cmd);
void		set_pipes(t_data *data, t_cmd *cmd);
void		close_pipes(t_cmd **root, t_cmd *cmd, t_cmd *last);
void		close_fd(t_data *data, t_cmd *cmd);
int			init_io_redir(t_data *data);

/**		var.c			**/
int			is_valid_var(char *str);
char		*get_var(t_data *data, char *str);
int			var_expander(t_data *data, char *str, t_token **token);

/**		var_expander	**/
void		var_expand_exitcode(t_data *data, char **ret, int *i);
int			var_expand_valid(t_data *data, char **ret, int *i);
int			var_expand_dollar(t_data *data, char **ret, int *i, t_token *token);

/**		var_utils.c		**/
char		*str_replace(char *src, int r_index, int n, char *str);
char		*str_replace_strs(char **src, int r_index, int n, char *str);

/**			export			**/
void		env_update(t_data *data);
int			set_in_env(t_data *data, char *variable, char **variable_split);
void		end_split(char **variable_split);
int			set_in_export(t_data *data, char *variable, t_cmd *cmd);
int			execute_export(t_data *data, t_cmd *cmd);
int			print_export(t_data *data);
void		sort_export_list(t_data *data);
t_cmd		*find_export_command(t_data *data);
void		env_to_export(t_data *data);
int			ft_export(t_data *data);

/**		export_utils	**/
t_export	*ft_lstnew_export(char *key, char *value, int flag);
void		ft_lstadd_back_export(t_export **lst, t_export *new_elem);
void		free_export_list(t_export **export_lst);
void		insert_sorted(t_export **sorted, t_export *new_export);
char		**ft_split2(char *s, char c);
int			ft_wordsize(char *s, char c, int pos);
void		free_tabs(char **tab);
int			key_is_valid(char *chaine);
char		*add_quotes(char *str);
int			export_key_exists(t_export *export_root, char *key_to_check);
void		remove_export(t_data *data, const char *key_to_remove);
int			env_key_exists(t_env *env, char *key_to_check);
void		remove_env(t_data *data, const char *key_to_remove);
void		ft_lstadd_back_env(t_env **lst, t_env *new_elem);
t_env		*ft_lstnew_env(char *key, char *value);
void		free_env_lst(t_env **cpy);

/**	temp	**/
int			handle_parent_directory(void);
int			handle_previous_directory(t_data *data, char **old_pwd);
void		update_oldpwd(char **old_pwd, char *new_pwd);
int			handle_home_directory(t_data *data, const char *dir,
				size_t full_path_len);

/**	unset	**/
int			ft_lstsize_env(t_env **lst);
void		env_update(t_data *data);
void		execute_unset(t_data *data, t_cmd *cmd, int i);
void		execute_env(t_data *data, t_cmd *cmd);
t_cmd		*find_unset_command(t_data *data);
int			ft_unset(t_data *data);

/**		dummies.c			**/
int			ft_true(void);
int			ft_false(void);
int			print_envp(t_data *data);

/**		free.c				**/
void		free_child(t_data *data);
void		free_heredoc_child(t_data *data);
void		free_subshell(t_data *data);
void		data_cleanup(t_data *data);
void		free_shell(t_data *data);
int			free_data(t_data *data);
int			free_return(int return_value, void *ptr_1, void *ptr_2, void *ptr3);
void		multi_free(void *ptr_1, void *ptr_2, void *ptr_3, void *ptr_4);

/**		parse_near_quote.c	**/
void		parse_near_quote(t_data *data);
char		*prompt_pwd(t_data *data);
void		free_envv(t_data *data);
void		free_set_in(char *key, char *value, char **variable_split);
void		invalid_export_print(char *key, char *value, char **variable_split);
void		env_update_utils(t_env *current_env, size_t key_len, t_data *data);
int			set_in_export_utils(t_data *data, char *key,
				char *value, t_cmd *cmd);
void		env_assign(char ***variable_split, char *variable, char **key,
				char **value);
/***		unsorted		***/
char		*get_cmd(char *cmd, char **env_p);
char		**get_path(char **envv);
char		*ft_readline(char *str);
void		ft_lstadd_back_env(t_env **lst, t_env *new_node);
void		free_env_list(t_env **env);
void		free_var(t_data *data, t_cmd *cmd);
void		print_token(t_token **root);
void		free_token(t_data *data);
char		**ft_split_noquote(char *str, char c);
int			var_is_multiple(char *var);
char		*chrtostr(char c);
int			check_error_raw(t_data *data);
char		*ft_strs_join(char **tab);

char		*ft_getenvcpy(t_data *data, char *key);
int			cd_env_update(t_data *data, size_t i, size_t env_count);
int			cd_env_update_utils(t_data *data, size_t i, size_t env_count);
void		envcpy_update_utils(t_data *data, t_env *new_node);
#endif
