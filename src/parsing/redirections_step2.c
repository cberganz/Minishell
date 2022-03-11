/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_step2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 20:02:39 by rbicanic          #+#    #+#             */
/*   Updated: 2022/03/11 01:36:39 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

uint8_t pipe_redirection_management(t_pipe_command *cmd)
{
	int		i;
	void	*ret;

	i = 0;
	ret = (void *)1;
	while (cmd->cmd_content[i])
	{
		if (cmd->cmd_content[i] == '"')
			i += quote_len(&(cmd->cmd_content)[i], '"') + 1;
		else if (cmd->cmd_content[i] == '\'')
			i += quote_len(&(cmd->cmd_content)[i], '\'') + 1;
		else if (!ft_strncmp(&cmd->cmd_content[i], ">>", 2))
			ret = out_redirection_parsing(cmd, ">>", i);
		else if (!ft_strncmp(&cmd->cmd_content[i], "<<", 2))
			ret = in_redirection_parsing(cmd, "<<", i);
		else if (cmd->cmd_content[i] == '>')
			ret = out_redirection_parsing(cmd, ">", i);
		else if (cmd->cmd_content[i] == '<')
			ret = in_redirection_parsing(cmd, "<", i);
		else if (cmd->cmd_content[i])
			i++;
		if (ret == NULL)
			return (1);
	}
	return (0);
}

int	cmd_redirection_management(t_list *list)
{
	t_list	*tmp;

	if (!list)
		return (0);
	tmp = list;
	while (tmp)
	{
		if (pipe_redirection_management((t_pipe_command *)tmp->content))
			return (1);
		((t_pipe_command *)tmp->content)->cmd_content =
			ft_strtrim(((t_pipe_command *)tmp->content)->cmd_content, " ", LOOP);
		if (!((t_pipe_command *)tmp->content)->cmd_content)
			return (1);
		/*
		** A delete // en bas => permet d'eviter les fds ouverts
		*/
			// return (print_message(strerror(errno), RED, MALLOC_ERR), 1);
		// /*
		// ** A delete // en bas => permet d'eviter les fds ouverts
		// */
		// /**/if (((t_pipe_command *)tmp->content)->fd_redirection[FD_IN] != 0 && ((t_pipe_command *)tmp->content)->fd_redirection[FD_IN] != 1)
		// /**/	close(((t_pipe_command *)tmp->content)->fd_redirection[FD_IN]);
		// /**/if (((t_pipe_command *)tmp->content)->fd_redirection[FD_OUT] != 0 && ((t_pipe_command *)tmp->content)->fd_redirection[FD_OUT] != 1)
		// /**/	close(((t_pipe_command *)tmp->content)->fd_redirection[FD_OUT]);
		/*
		** A delete // en haut => permet d'eviter les fds ouverts
		*/
		tmp = tmp->next;
	}
	return (0);
}
