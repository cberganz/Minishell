/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 03:29:34 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/14 18:26:36 by rbicanic         ###   ########.fr       */
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
	while (*str && *str != '=' && *str != '_')
	{
		if (!ft_strncmp(str, "+=", 2))
			return (2);
		if (!ft_isalnum(*str))
			return (0);
		str++;
	}
	return (1);
}

char	*env_variable_exist(char *envp[], char *param, size_t var_name_len)
{
	while (*envp)
	{
		if (!ft_strncmp(param, *envp, var_name_len))
			return (*envp);
		envp++;
	}
	return (*envp);
}

char	*ft_strndup(const char *s, int garbage, size_t len)
{
	char	*sdup;

	if (mem_alloc(len * sizeof(char), (void **)&sdup, garbage))
		return (NULL);
	ft_strlcpy(sdup, s, len);
	return (sdup);
}

uint8_t	add_var_to_env(char *exec_args, char **envp[], char *equal_ptr, int ret_alnum)
{
	char	*var_name;
	char	*var_value;
	char	*var_ptr_env;

	var_ptr_env = NULL;
	if (ret_alnum == 2)
	{
		var_name = ft_strndup(exec_args, MAIN, (equal_ptr - exec_args));
		var_ptr_env = env_variable_exist(*envp, var_name, ft_strlen(var_name));
	}	
	else
		var_name = ft_strndup(exec_args, MAIN, (equal_ptr - exec_args) + 1);
	if (var_name == NULL)
		return (1);
	if (ret_alnum == 2 && var_ptr_env)
		var_value = ft_strjoin(&var_ptr_env[ft_strlen(var_name) + 1], equal_ptr + 1, MAIN);//ft_strndup(equal_ptr + 1, MAIN, ft_strlen(equal_ptr + 1) + 1);
	else
		var_value = ft_strndup(equal_ptr + 1, MAIN, ft_strlen(equal_ptr + 1) + 1);
	if (var_value == NULL)
		return (1);
	set_env(var_name, var_value, envp);
	return (0);
}

int	builtin_export(char **exec_args, char **envp[])
{
	char	*equal_ptr;
	int		ret_name_alnum;
	int		error;

	error = 0;
	if (!(*exec_args))
	{
		export_print_env(*envp);
		return (0);
	}
	while (*exec_args)
	{
		ret_name_alnum = ft_var_name_isalnum(*exec_args);
		if ((!ft_isalpha(*exec_args[0]) && *exec_args[0] != '_') || !ret_name_alnum)
		{
			print_message("Minishell: export: «", RED, 0);// revoir message erreur en anglais
			print_message(*exec_args, RED, 0);// revoir message erreur en anglais
			print_message("» : not a valid identifier\n", RED, 0);// revoir message erreur en anglais
			error = 1;// pas sur de retourner 1 tout de suite peut etre continuer sur les autres ARGS
		}
		equal_ptr = ft_strchr(*exec_args, '=');
		if (equal_ptr && add_var_to_env(*exec_args, envp, equal_ptr, ret_name_alnum))
			return (1);
		exec_args++;
	}
	// if (exit)
	// 	free_and_exit(error);
	return (error);
}
