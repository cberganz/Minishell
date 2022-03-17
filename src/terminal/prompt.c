/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 12:04:29 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/17 03:18:13 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_prompt(char **envp[])
{
	char	*prompt;
	char	*home;

	home = getenv("HOME");
	prompt = get_env("PWD", envp);//getcwd(NULL, 0); // TEMPORARY SOLUTION
	//prompt = "Minishell"; // TEMPORARY SOLUTION
	//printf("%sTEST\n", prompt); prompt est null ici quand le directory dans lequel on est a ete rm -rf dans la precedente commande
	if (!prompt)
		prompt = "Minishell";
//		free_and_exit(MALLOC_ERR);
	//garbage_addptr(prompt, LOOP); TEMPORARY SOLUTION
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
