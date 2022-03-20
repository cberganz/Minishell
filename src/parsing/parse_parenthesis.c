/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_parenthesis.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <cberganz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 20:14:31 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/20 00:16:46 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	quotes_len(char *input, int i)
{
	int	save;

	save = i;
	if (input[i] == '\"')
		i += quote_len(&input[i], '\"') + 1;
	else if (input[i] == '\'')
		i += quote_len(&input[i], '\'') + 1;
	return (i - save);
}

static uint8_t is_meta(char *sub_input, int i, int direction)
{
	if (direction == PREVIOUS)
	{
		while (--i >= 0)
		{
			if (sub_input[i] == ' ' || sub_input[i] == '(')
				continue ;
			else if (i > 0 && sub_input[i] ==  '|' && sub_input[i - 1] ==  '|')
				return (1);
			else if (sub_input[i] ==  '&')
				return (1);
			else
				return (0);
		}
		if (i < 0)
			return (1);
	}
	else if (direction == NEXT)
	{
		while (sub_input[++i])
		{
			if (sub_input[i] == ' ' || sub_input[i] == ')')
				continue ;
			else if (sub_input[i] ==  '|' && sub_input[i + 1] ==  '|')
				return (1);
			else if (sub_input[i] ==  '&')
				return (1);
			else
				return (0);
		}
		if (!sub_input[i])
			return (1);
	}
	return (1);
}

static uint8_t	open_parenthesis(char *input)
{
	int	i;
	int	j;
	int	left_nbr;
	int	right_nbr;
	int	is_command;

	i = 0;
	j = 0;
	left_nbr = 0;
	right_nbr = 0;
	is_command = 0;
	while (input[i])
	{
		i += quotes_len(input, i);
		if (input[i] == '(')
		{
			left_nbr++;
			if (input[i + 1] == ')')
				return (print_first_check_error(NEAR_TOKEN_ERR_MSG, ")", &input));
			j = i;
			while (input[++j] && input[j] != ')')
			{
				if (input[j] != ' ')
					break ;
			}
			if (!input[j])
				return (print_first_check_error(PARENTHESIS_ERR_MSG, NULL, &input));
			if (input[j] == ')')
				return (print_first_check_error(NEAR_TOKEN_ERR_MSG, ")", &input));
		}
		else if (input[i] == ')')
			right_nbr++;
		i++;
	}
	if (left_nbr == right_nbr)
		return (0);
	return (print_first_check_error(PARENTHESIS_ERR_MSG, NULL, &input));
}

static char	*get_token(char *input, int i)
{
	int		size;
	int		ret;
	int		cpy;
	char	*token;

	size = 0;
	while (input[++i] && input[i] == ' ')
		;
	if (input[i] == '\0')
		return ("newline");
	ret = 0;
//	if (ft_ischarset(input[i], "&|", NULL))
//		return ("(");
	while (input[i] && !ft_ischarset(input[i], ") ", NULL))
	{
		if (input[i] == '\"' || input[i] == '\'')
		{
			ret += quotes_len(input, i) - 1;
			size += quotes_len(input, i) - 1;
			i += quotes_len(input, i) - 1;
		}	
		size++;
		ret++;
		i++;
	}
	if (mem_alloc(size + 1, (void **)&token, LOOP))
		print_message("minishell: Allocation error\n", RED, 1);
	cpy = 0;
	i -= ret;
	while (cpy < size)
	{
		token[cpy] = input[i];
		cpy++;
		i++;
	}
	token[cpy] = '\0';
	return (token);
}

uint8_t	parenthesis_checker(char *input)
{
	int	i;

	i = 0;
	if (open_parenthesis(input))
		return (1);
	while (input[i])
	{
		i += quotes_len(input, i);
		if (input[i] == '(' && !is_meta(input, i, PREVIOUS))
			return (print_first_check_error(NEAR_TOKEN_ERR_MSG, get_token(input, i), &input));
		else if (input[i] == ')' && !is_meta(input, i, NEXT))
			return (print_first_check_error(NEAR_TOKEN_ERR_MSG, get_token(input, i), &input));
		i++;
	}
	return (0);
}
