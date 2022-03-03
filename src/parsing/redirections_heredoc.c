/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_heredoc.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 00:44:17 by rbicanic          #+#    #+#             */
/*   Updated: 2022/03/03 10:47:27 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// a mettre dans une fonction qui parcours tous les maillons d'une liste chainee et sa sous liste
// cad vraiment juste apres le parsing

//test read document 
int read_fd(int fd)
{
    char c;
	int n;

	n = 1;
    while (n > 0)
	{
		n = read(fd, &c, 1);
		if (n < 0) 
			printf("READ ERROR\n");
    	write(1, &c, 1);
	}
	if (n < 0) 
	{
        perror("read");
        return 1;
    }
    return 0;
}
//test read document 

u_int8_t	check_eof_heredoc(char *input)
{
	if (!input)
	{
		if (g_status == 130)
			return (1);
		else
		{
			printf(RED "Minishell: warning: here-document at line 1 delimited by end-of-file\n" RESET);
			return (1);
		}
	}
	return (0);
}

uint8_t	write_in_tmp_file(t_pipe_command *cmd, int i)
{
	int		len_of_file;
	char	*end_word;
	char	*input;
	int		save_status;

	input = "";
	len_of_file = file_len(&cmd->cmd_content[i]);
	end_word = ft_filedup(&cmd->cmd_content[i], len_of_file);
	save_status = g_status;
	g_status = -255;
	input = garbage_addptr(readline("> "), LOOP);
	if (check_eof_heredoc(input))
		return (1);
	while (!ft_strequ(input, end_word))
	{
		write(cmd->fd_tmp, input, ft_strlen(input));
		write(cmd->fd_tmp, "\n", 1);
		input = garbage_addptr(readline("> "), LOOP);
		if (check_eof_heredoc(input))
			return (1);
	}
	g_status = save_status;
	close(cmd->fd_tmp);
	return (0);
}

uint8_t	heredoc_exec(t_pipe_command *cmd, int i, int file_nbr)
{
	char	*tmp_file;

	if (cmd->fd_tmp)
		close(cmd->fd_tmp);
	while (cmd->cmd_content[i] == ' ' || cmd->cmd_content[i] == '\t')
		i++;
	tmp_file = ft_strjoin("file_test/tmp_file", ft_itoa(file_nbr, LOOP), LOOP);
	cmd->fd_tmp = open(tmp_file, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (cmd->fd_tmp == -1)
		printf("Error opening file");
	if (write_in_tmp_file(cmd, i))
		return (1);
	cmd->fd_tmp = open(tmp_file, O_RDONLY);
	unlink(tmp_file);
	return (0);
}

uint8_t find_heredoc(t_pipe_command *cmd, int file_nbr)
{
	int		i;

	i = 0;
	while (cmd->cmd_content[i])
	{
		if (cmd->cmd_content[i] == '"')
			i += quote_len(&(cmd->cmd_content)[i], '"') + 1;
		else if (cmd->cmd_content[i] == '\'')
			i += quote_len(&(cmd->cmd_content)[i], '\'') + 1;
		else if (!ft_strncmp(&cmd->cmd_content[i], "<<", 2))
		{
			i += 2;
			if (heredoc_exec(cmd, i, file_nbr))
				return (1);
		}
		else if (cmd->cmd_content[i])
			i++;
	}
	return (0);
}

uint8_t	heredoc_management(t_list *list)
{
	t_list	*tmp;
	int		y;
	int 	x;

	if (!list)
		return (0);
	y = 0;
	x = 0;
	while (list)
	{
		tmp = ((t_command *)list->content)->command_list;
		while (tmp)
		{
			if (find_heredoc((t_pipe_command *)tmp->content, x * 10 + y))
				return (1);
			tmp = tmp->next;
			y++;
		}
		list = list->next;
		x++;
	}
	return (0);
}

void	close_heredoc_fds(t_list *list)// retours errors ne pas oublier
{
	t_list	*tmp;

	if (!list)
		return ;
	while (list)
	{
		tmp = ((t_command *)list->content)->command_list;
		while (tmp)
		{
			if (((t_pipe_command *)tmp->content)->fd_tmp)
				close(((t_pipe_command *)tmp->content)->fd_tmp);
			tmp = tmp->next;
		}
		list = list->next;
	}
}
