/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 12:25:36 by rbicanic          #+#    #+#             */
/*   Updated: 2022/03/08 11:15:07 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** VERIFIER QUE LE LA MEMOIRE EST COMPLETEMENT FREE !!!! 
*/

static int	ft_var_name_isalnum(char *str)
{
	//voir si besoin de tronquer a priori pas
	if (!*str)
		return (0);
	while (*str)// underscore possible
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (0);
		str++;
	}
	return (1);
}

char	**remove_var(char **envp[], char *env_var)
{
	int				j;
	unsigned long	line;
	char			**new_envp;

	line = 0;
	while ((*envp)[line])
		line++;
	// new_envp = mem_alloc(i * sizeof(char *), (void **)&new_envp, MAIN);
	if (mem_alloc(line * sizeof(char *), (void **)&new_envp, MAIN))
		print_message("minishell: Allocation error.\n", RED, 1);
	if (new_envp == NULL)
		return (NULL);
	line = 0;
	j = 0;
	while ((*envp)[line])
	{
		if ((*envp)[line] == env_var)
			line++;
		new_envp[j] = (*envp)[line];
		j++;
		line++;
	}
	new_envp[j] = NULL;
	return (new_envp);
}

int	builtin_unset(char **exec_args, int exit, char **envp[])
{
	char	*env_var;

	while (*exec_args)
	{
		if (!ft_isalpha(*exec_args[0]) || !ft_var_name_isalnum(*exec_args))
		{
			print_message("Minishell: export: « PARAM » : identifiant non valable\n", RED, 0);// revoir message erreur en anglais
			if (exit)
				free_and_exit(1);
			return (1);// pas sur de retourner 1 tout de suite peut etre continuer sur les autres ARGS
		}
		env_var = env_variable_exist(*envp, *exec_args, ft_strlen(*exec_args));
		if (env_var)
			*envp = remove_var(envp, env_var);//secure
		exec_args++;
	}
	if (exit)
		free_and_exit(0);
	return (0);
}
