/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 15:20:14 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/28 15:20:15 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINE_H
# define DEFINE_H

# define USAGE_MSG "Correct use is ./minishell or ./minishell \
-c \"commands to be executed\""
# define PROG_NAME "minishell: "
# define ERR_FORK "minishell: error creating child process\n"
# define EXPORT_MSG "declare -x"
# define HEREDOC_EOF "minishell: warning: here-document \
delimited by end-of-file (wanted `"
# define SYNTAX_ERROR 2
# define CMD_ERR_FKO 127
# define CMD_ERR_XKO 126
# define NONE 0

#endif
