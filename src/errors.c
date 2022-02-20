/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 19:40:20 by rbicanic          #+#    #+#             */
/*   Updated: 2022/02/19 22:12:32 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*unexpected_next_token(char *str, int index)
{
	while (str[index] == ' ')
		index++;
	if (!ft_strncmp(&str[index], "||", 2))
		return ("||");
	else if (!ft_strncmp(&str[index], "&&", 2))
		return ("&&");
	else if (str[index] == '|')
		return ("|");
	else if (!ft_strncmp(&str[index], ">>", 2))
		return (">>");
	else if (!ft_strncmp(&str[index], "<<", 2))
		return ("<<");
	else if (str[index] == '>')
		return (">");
	else if (str[index] == '<')
		return ("<");
	else if (str[index] == '\0')//test si pas \n
		return ("newline");
	return (NULL);
}

uint8_t	unexpected_prev_token(char *str, int index)
{
	if (index > 0)
		index--;//verifier
	while (index > 0 && str[index] == ' ')
		index--;
	if (str[index] == '|' || str[index] == '&'
		|| str[index] == '>' || str[index] == '<' || str[index] == ' ')
		return (1);
	return (0);
}

char	*charset_token_error(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (!ft_strncmp(&input[i], "||", 2))
		{
			if (unexpected_prev_token(input, i))
				return ("||");
			i += 1;
		}
		else if (!ft_strncmp(&input[i], "&&", 2) && unexpected_prev_token(input, i))
			return ("&&");
		else if (input[i] == '|' && unexpected_prev_token(input, i))
			return ("|");
// potentiellement ajouter les >>> et <<< pour respecter ex
		else if (!ft_strncmp(&input[i], ">>", 2))
		{
			if (unexpected_next_token(input, i + 2))
				return (unexpected_next_token(input, i + 2));
			i += 2;
		}
		else if (!ft_strncmp(&input[i], "<<", 2) && unexpected_next_token(input, i + 2))
		{
			if (unexpected_next_token(input, i + 2))
				return (unexpected_next_token(input, i + 2));
			i += 2;
		}
		else if (input[i] == '>' && unexpected_next_token(input, i + 1))
			return (unexpected_next_token(input, i + 1));
		else if (input[i] == '<' && unexpected_next_token(input, i + 1))
			return (unexpected_next_token(input, i + 1));
		i++;
	}
	return (NULL);
}

uint8_t	near_unexpected_token_error(char **input, char **shell_prompt)
{
	char	*unexpected_token;

	unexpected_token = charset_token_error(*input);
	if (unexpected_token)
	{
		printf(RED "Minishell: syntax error near unexpected token `%s'\n" RESET,
			unexpected_token);
		*input = "";
		g_status = 1;
		*shell_prompt = create_prompt();
		return (1);
	}
	return (0);
}