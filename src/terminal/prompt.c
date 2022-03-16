/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 12:04:29 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/16 17:12:33 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_prompt(void)
{
	char	*prompt;
	char	*home;

	home = getenv("HOME");
	prompt = getcwd(NULL, 0);
	if (!prompt)
		free_and_exit(MALLOC_ERR);
	garbage_addptr(prompt, LOOP);
	if (home && ft_strstr(prompt, home))
	{
		prompt[ft_strlen(home) - 1] = '~';
		prompt = &prompt[ft_strlen(home) - 1];
	}
	if (g_status)
		prompt = ft_strjoin("\001\e[31;1m\002> \001\e[35m\002", prompt, LOOP);
	else
		prompt = ft_strjoin("\001\e[32;1m\002> \001\e[35m\002", prompt, LOOP);
	prompt = ft_strjoin(prompt, "$ \001\e[0m\002", LOOP);
	if (!prompt)
		free_and_exit(MALLOC_ERR);
	return (prompt);
}
