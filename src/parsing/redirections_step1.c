/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_step1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 20:02:39 by rbicanic          #+#    #+#             */
/*   Updated: 2022/03/03 10:46:12 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	file_len(char *s)
{
	int		len;

	len = 0;
	while (s[len] && s[len] != ' ' && s[len] != '|' &&
			s[len] != '&' && s[len] != '<' && s[len] != '>')
	{
		if (s[len] == '"')
		{
			len += quote_len(&s[len], '"') + 1;
		}
		else if (s[len] == '\'')
			len += quote_len(&s[len], '\'') + 1;
		else
			len++;
	}
	return (len);
}

char	*ft_filedup(char *s, int len_of_file)
{
	char	*dest;
	int		i;

	if (mem_alloc(sizeof(char) * (len_of_file + 1), (void **)&dest, LOOP))
		return (NULL);
	i = 0;
	while (i < len_of_file)
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

void	remove_file(int	len_of_file, char *s)
{
	int	j;

	j = 0;
	while (j < len_of_file)
	{
		ft_strcpy(&s[0], &s[1]);
		j++;
	}
}

void	*out_redirection_parsing(t_pipe_command *cmd, char *operator, int i)
{
	int	len_of_file;

	if (cmd->fd_redirection[FD_OUT] != 0 && cmd->fd_redirection[FD_OUT] != 1)
		close(cmd->fd_redirection[FD_OUT]);
	cmd->outfile_operator = operator;
	while (cmd->cmd_content[i] == operator[0] || cmd->cmd_content[i] == ' ')
		ft_strcpy(&cmd->cmd_content[i], &cmd->cmd_content[i + 1]);
	len_of_file = file_len(&cmd->cmd_content[i]);
	cmd->outfile = ft_filedup(&cmd->cmd_content[i], len_of_file);
	if (cmd->outfile == NULL)
		return (NULL);
	//check access si erreur sortir + print error avec exit + free du garbage
	remove_file(len_of_file, &cmd->cmd_content[i]);
	if (!ft_strncmp(operator, ">>", 2))
		cmd->fd_redirection[FD_OUT] = open(cmd->outfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
	else if (!ft_strncmp(operator, ">", 1))
		cmd->fd_redirection[FD_OUT] = open(cmd->outfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (cmd->fd_redirection[FD_OUT] == -1)
		errno_file_error(cmd->outfile, 1);
	return ((void *)1);
}

/*
*** ajout d'un access pour verifier le droits des fichiers et l'existence des ficiers
*** peut etre faire les redirections dans les childs a cause de ca => tres probable
*/

void	*in_redirection_parsing(t_pipe_command *cmd, char *operator, int i)
{
	int	len_of_file;

	if (cmd->fd_redirection[FD_IN] != 0 && cmd->fd_redirection[FD_IN] != 1)
		close(cmd->fd_redirection[FD_OUT]);
	cmd->infile_operator = operator;
	while (cmd->cmd_content[i] == operator[0] || cmd->cmd_content[i] == ' ')
		ft_strcpy(&cmd->cmd_content[i], &cmd->cmd_content[i + 1]);
	len_of_file = file_len(&cmd->cmd_content[i]);
	if (ft_strncmp(operator, "<<", 2))
	{
		cmd->infile = ft_filedup(&cmd->cmd_content[i], len_of_file);
		if (cmd->infile == NULL)
			return (NULL);
	
		//check access si erreur sortir + print error avec exit + free du garbage
		cmd->fd_redirection[FD_IN] = open(cmd->infile, O_RDONLY);
		if (cmd->fd_redirection[FD_IN] == -1)
			errno_file_error(cmd->infile, 1);
	}
	else
		cmd->fd_redirection[FD_IN] = cmd->fd_tmp;
	remove_file(len_of_file, &cmd->cmd_content[i]);
	return ((void *)1);
}
