/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_heredoc.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 00:44:17 by rbicanic          #+#    #+#             */
/*   Updated: 2022/03/01 20:09:11 by rbicanic         ###   ########.fr       */
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

void	write_in_tmp_file(t_pipe_command *cmd, int i)
{
	int		len_of_file;
	char	*end_word;
	char	*input;

	input = "";
	len_of_file = file_len(&cmd->cmd_content[i]);
	end_word = ft_filedup(&cmd->cmd_content[i], len_of_file);
	input = garbage_addptr(readline("> "));
	while (!ft_strequ(input, end_word))
	{
		write(cmd->fd_tmp, input, ft_strlen(input));
		write(cmd->fd_tmp, "\n", 1);
		input = garbage_addptr(readline("> "));
		if (!input)
			eof_exit();
	}
	close(cmd->fd_tmp);
}

void	heredoc_exec(t_pipe_command *cmd, int i, int file_nbr)
{
	char	*tmp_file;

	if (cmd->fd_tmp)
		close(cmd->fd_tmp);
	while (cmd->cmd_content[i] == ' ' || cmd->cmd_content[i] == '\t')
		i++;
	tmp_file = ft_strjoin("file_test/tmp_file", ft_itoa(file_nbr));
	cmd->fd_tmp = open(tmp_file, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (cmd->fd_tmp == -1)
		printf("Error opening file");
	write_in_tmp_file(cmd, i);
	cmd->fd_tmp = open(tmp_file, O_RDONLY);
	unlink(tmp_file);
	read_fd(cmd->fd_tmp);
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
			heredoc_exec(cmd, i, file_nbr);
		}
		else if (cmd->cmd_content[i])
			i++;
	}
	return (0);
}

int	heredoc_management(t_list *list)
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
			if (find_heredoc((t_pipe_command *)list->content, x * 10 + y))
				return (1);
			tmp = tmp->next;
			y++;
		}
		list = list->next;
		x++;
	}
	return (0);
}

void	close_heredoc_fds(t_list *list)
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
