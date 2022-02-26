/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_remove_quotes.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/26 19:18:34 by rbicanic          #+#    #+#             */
/*   Updated: 2022/02/26 19:24:21 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_remove_quotes(char *str)
{
	int	i;
	int	quote_pos;

	i = 0;
	while (str[i])
	{	
		if (str[i] == '"')
		{
			ft_strcpy(&str[i], &str[i + 1]);
			i += quote_len(&str[i], '"');
		}
		else if (str[i] == '\'')
		{

			i += quote_len(&str[i], '\'') + 1;
		}
		i++;
	}
}