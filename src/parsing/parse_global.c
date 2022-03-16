/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_global.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 13:58:56 by rbicanic          #+#    #+#             */
/*   Updated: 2022/03/16 15:42:39 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*global_parsing(char *input)
{
	t_list	*list;

	remove_comments(&input);
	list = parse_step1(input);
	if (!list)
		return (NULL);
	if (!single_pipe_parsing(&list))
		return (NULL); // FERMER LE PROCESS DEPUIS LA FONCTION EN CAS D'ERREUR MALLOC (pour réserver le retour NULL au cas continue)
	return (list);
}

void	command_parsing(t_list *command_list, char **envp[])
{
	tilde_expansion(command_list, envp);
	cmd_redirection_management(command_list, envp);
	variable_expansion(command_list, envp);
}
