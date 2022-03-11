/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_step5_removequotes.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/26 19:18:34 by rbicanic          #+#    #+#             */
/*   Updated: 2022/03/10 23:50:14 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//static void	remove_quotes_str(char **str) // To test, found some errors
//{
//	int	i;
//
//	i = 0;
//	while (*(*str + i))
//	{
//		if ((*(*str + i) == '"' && *(*str + i + 1) == '"')
//			|| (*(*str + i) == '\'' && *(*str + i + 1) == '\''))
//			ft_strcpy(&(*(*str + i)), &(*(*str + i + 2)));
//		else if (*(*str + i) == '"')
//		{
//			ft_strcpy(&(*(*str + i)), &(*(*str + i + 1)));
//			i += quote_len(&(*(*str + i)), '"');
//			ft_strcpy(&(*(*str + i)), &(*(*str + i + 1)));
//			i--;
//		}
//		else if (*(*str + i) == '\'')
//		{
//			ft_strcpy(&(*(*str + i)), &(*(*str + i + 1)));
//			i += quote_len(&(*(*str + i)), '\'');
//			ft_strcpy(&(*(*str + i)), &(*(*str + i + 1)));
//			i--;
//		}
//		if (*(*str + i) && *(*str + i) != '\'' && *(*str + i) != '\"')
//			i++;
//	}
//}

static void	quotes(char *str, char *nstr, int *str_i, int *nstr_i)
{
	if (str[*str_i] && str[*str_i] == '"')
	{
		(*str_i)++;
		while (str[*str_i] && str[*str_i] != '"')
		{
			nstr[*nstr_i] = str[*str_i];
			(*nstr_i)++;
			(*str_i)++;
		}
		(*str_i)++;
	}
	else if (str[*str_i] && str[*str_i] == '\'')
	{
		(*str_i)++;
		while (str[*str_i] && str[*str_i] != '\'')
		{
			nstr[*nstr_i] = str[*str_i];
			(*nstr_i)++;
			(*str_i)++;
		}
		(*str_i)++;
	}
}

void	remove_quotes_str(char **str)
{
	char	*nstr;
	int		str_i;
	int		nstr_i;

	str_i = 0;
	nstr_i = 0;
	if (mem_alloc(ft_strlen(*str), (void **)&nstr, LOOP))
		print_message("Allocation error.\n", RED, MALLOC_ERR);
	while ((*str)[str_i])
	{
		if ((*str)[str_i] && ((*str)[str_i] == '\'' || (*str)[str_i] == '"'))
			quotes(*str, nstr, &str_i, &nstr_i);
		else if ((*str)[str_i] && (*str)[str_i] != '\'' && (*str)[str_i] != '"')
		{
			nstr[nstr_i] = (*str)[str_i];
			str_i++;
			nstr_i++;
		}
	}
	nstr[nstr_i] = '\0';
	//mem_remove(&(*str));
	*str = nstr;
}

uint8_t	isquote_in(char *str)
{
	while (*str)
	{
		if (ft_ischarset(*str, "\'\"", NULL))
			return (1);
		str++;
	}
	return (0);
}

void	remove_quotes_list(t_list *command_list)
{
	int	i;

	while (command_list)// sur ""|'' ne rentre pas dans la boucle pour '' (command_list = NULL)
	{
		if (((t_pipe_command *)command_list->content)->exec_args)
		{
			i = 0;
			while (((t_pipe_command *)command_list->content)->exec_args[i])
			{
				if (isquote_in(((t_pipe_command *)command_list->content)->exec_args[i]))
					remove_quotes_str(&((t_pipe_command *)command_list->content)->exec_args[i]);
				i++;
			}
		}
		command_list = command_list->next;
	}
}
