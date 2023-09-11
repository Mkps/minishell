#include "../include/minishell.h"

int	check_io_error(t_token **root)
{
	t_token	*tmp;

	tmp = *root;
	while (tmp)
	{
		if (token_is_io(tmp))
		{
			if (tmp->next && (tmp->next->token_type != WORD && !token_is_quote(tmp->next)))
			{
				ft_printf("syntax error near unexpected token `newline'\n");
				return (EXIT_FAILURE);
			}
		}
		tmp = tmp->next;
	}
	return (EXIT_SUCCESS);
}