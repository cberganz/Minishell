/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <cberganz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 04:04:52 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/03 16:43:41 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env(char *var, char **envp[])
{
	int	size;
	int	line;

	line = 0;
	while ((*envp)[line])
	{
		size = 0;
		while ((*envp)[line][size] && (*envp)[line][size] != '=')
			size++;
		if (ft_strncmp((*envp)[line], var, size) == 0)
			return (ft_strdup(&(*envp)[line][size + 1], LOOP));
		line++;
	}
	return ("");
}

void	set_env(char *var, char *content, char **envp[])
{
	char	*tmp;
	int		line;

	line = 0;
	tmp = ft_strjoin(var, "=", MAIN);
	tmp = ft_strjoin(tmp, content, MAIN);
	if (!tmp)
		print_message("export: Allocation error.\n", RED, 1);
	while ((*envp)[line])
	{
		if (ft_strnequ((*envp)[line], var, ft_strlen(var)))
		{
			mem_remove((*envp)[line], MAIN);
			(*envp)[line] = tmp;
			break ;
		}
		line++;
	}
	if (!(*envp)[line])
	{
		ft_strlcpy((*envp)[line], var, ft_strlen(var));
		ft_strlcat((*envp)[line], "=", 1);
		ft_strlcat((*envp)[line], content, ft_strlen(content));
	}
}

char	*path_troncate(char *s, char *to_troncate)
{
	int	i;

	i = 0;
	while (s[i] && !ft_strequ(&s[i], to_troncate))
		i++;
	while (s[i] != '\0')
	{
		s[i] = '\0';
		i++;
	}
	return (s);
}

