/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_heredoc_loop.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 00:44:17 by rbicanic          #+#    #+#             */
/*   Updated: 2022/03/17 17:48:07 by cberganz         ###   ########.fr       */
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

static void	heredoc_loop(char *end_word, int var_expand, t_pipe_command *cmd, char ***envp)
{
	char	*input;

	input = "";
	if (isquote_in(end_word))
		remove_quotes_str(&end_word);
	input = garbage_addptr(readline("> "), LOOP);
	if (check_eof_heredoc(input))
		return ;
	if (ft_strequ(input, end_word))
		return ;
	redirection_var_expand(var_expand, &input, envp, "?_@#*-");
	while (!ft_strequ(input, end_word))
	{
		write(cmd->fd_tmp, input, ft_strlen(input));
		write(cmd->fd_tmp, "\n", 1);
		input = garbage_addptr(readline("> "), LOOP);
		if (check_eof_heredoc(input))
			break ;
		if (ft_strequ(input, end_word))
			break ;
		redirection_var_expand(var_expand, &input, envp, "?_@#*-");
	}
}

static void	write_in_tmp_file(t_pipe_command *cmd, int i, char ***envp)
{
	int		len_of_file;
	char	*end_word;
	int		save_status;
	int		var_expand;


	len_of_file = file_len(&cmd->cmd_content[i]);
	end_word = ft_filedup(&cmd->cmd_content[i], len_of_file);
	var_expand = 1;
	if (ft_strchr(end_word, '\'') || ft_strchr(end_word, '"'))
		var_expand = 0;
	save_status = g_status;
	g_status = -255;
	heredoc_loop(end_word, var_expand, cmd, envp);
	g_status = save_status;
	close(cmd->fd_tmp);
}

uint8_t	heredoc_exec(t_pipe_command *cmd, int i, char *file_nbr, char ***envp)
{
	char	*tmp_file;

	if (cmd->fd_tmp)
		close(cmd->fd_tmp);
	while (cmd->cmd_content[i] == ' ' || cmd->cmd_content[i] == '\t')
		i++;
	tmp_file = ft_strjoin("tmp_file", file_nbr, LOOP);
	if (!tmp_file)
		return (print_message(strerror(errno), RED, MALLOC_ERR), 1);
	cmd->fd_tmp = open(tmp_file, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (cmd->fd_tmp == -1)
		return (errno_file_error("tmpfile", 0), 1);
	write_in_tmp_file(cmd, i, envp);
	cmd->fd_tmp = open(tmp_file, O_RDONLY);
	if (cmd->fd_tmp == -1)
		return (errno_file_error("tmpfile", 0), 1);
	// read_fd(cmd->fd_tmp);
	unlink(tmp_file);
	return (0);
}

uint8_t find_heredoc(t_pipe_command *cmd, int file_nbr, char ***envp)
{
	int		i;
	char	*file_nbr_str;

	i = 0;
	file_nbr_str = ft_itoa(file_nbr, LOOP);
	if (!file_nbr_str)
		return (print_message(strerror(errno), RED, MALLOC_ERR), 1);
	while (cmd->cmd_content[i])
	{
		if (cmd->cmd_content[i] == '"')
			i += quote_len(&(cmd->cmd_content)[i], '"') + 1;
		else if (cmd->cmd_content[i] == '\'')
			i += quote_len(&(cmd->cmd_content)[i], '\'') + 1;
		else if (!ft_strncmp(&cmd->cmd_content[i], "<<", 2))
		{
			i += 2;
			if (heredoc_exec(cmd, i, file_nbr_str, envp))
				return (1);
		}
		else if (cmd->cmd_content[i])
			i++;
	}
	return (0);
}

uint8_t	heredoc_management(t_list *list, char ***envp)
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
			if (find_heredoc((t_pipe_command *)tmp->content, x * 10 + y, envp))
				return (1);
			tmp = tmp->next;
			y++;
		}
		list = list->next;
		x++;
	}
	return (0);
}
