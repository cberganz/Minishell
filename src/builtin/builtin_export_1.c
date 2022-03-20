/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 03:29:34 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/20 17:23:14 by rbicanic         ###   ########.fr       */
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

static void	handle_shlvl(int nb, char **envp[])
{
	char	*shlvl;

	shlvl = ft_itoa(ft_atoi(get_env("SHLVL", envp)) + nb, MAIN);
	if (!shlvl)
		print_message(MALLOC_ERR_MSG, RED, 1);
	set_env("SHLVL", shlvl, envp);
	mem_remove(shlvl, MAIN);
}

int	builtin_export(char **exec_args, char **envp[], int exit, int fd)
{
	int					ret_name_alnum;
	char				*equal_ptr;
	static t_list		*export_var = NULL;
	int					error;

	error = 0;
	handle_shlvl(1, envp);
	if (!export_var)
	{
		export_init_env(*envp, &export_var);
		if (export_var == NULL)
			return (handle_shlvl(-1, envp), -1);
	}
	if (!(*exec_args))
	{
		print_strs_fd(&export_var, fd);
		if (exit)
			free_and_exit(0);
		return (handle_shlvl(-1, envp), 0);
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
			exec_args++;
			continue ;
		}
		if (!add_el_to_export_list(&export_var, *exec_args))
			return (handle_shlvl(-1, envp), -1);
		equal_ptr = ft_strchr(*exec_args, '=');
		if (equal_ptr && add_var_to_env(*exec_args, envp, equal_ptr, ret_name_alnum))
			return (handle_shlvl(-1, envp), 1);
		exec_args++;
	}
	if (exit)
		free_and_exit(error);
	return (handle_shlvl(-1, envp), error);
}
