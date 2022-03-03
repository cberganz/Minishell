/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_heredoc.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 00:44:17 by rbicanic          #+#    #+#             */
/*   Updated: 2022/03/03 13:06:53 by rbicanic         ###   ########.fr       */
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


// remplacement de variables dans le heredoc

static uint8_t	previous_token_ismeta(char *command, int i)
{
	while (i > 0 && command[i] != ' ' && command[i] != '\t'
			&& command[i] != '>' && command[i] != '<')
		i--;
	while (i > 0 && (command[i] == ' ' || command[i] == '\t'))
		i--;
	if (i >= 0 && ft_ischarset(command[i], "<>", NULL))
	{
		if (command[i] == '>')
		{
			if (i > 0 && command[i - 1] == '>')
				return (2);
			return (1);
		}
		if (command[i] == '<')
		{
			if (i > 0 && command[i - 1] == '<')
			{
				return (4);
			}
			return (3);
		}
	}
	return (0);
}

static uint8_t	flag(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == '"')
		{
			i++;
			while (str[i] && str[i] != '"')
			{
				if (str[i] == '$' && ft_ischarset(str[i + 1], "?", ft_isalnum)
					&& previous_token_ismeta(str, i) != 4)
					return (1);
				i++;
			}
		}
		if (str[i] && str[i] == '\'')
		{
			i++;
			while (str[i] && str[i] != '\'')
				i++;
		}
		if (str[i] && str[i] == '$' && ft_ischarset(str[i + 1], "?@#*-_\'\"",
				ft_isalnum) && previous_token_ismeta(str, i) != 4)
			return (1);
		if (str[i])
			i++;
	}
	return (0);
}

static void	jump_quotes(char *cmd, int *double_quote, int *i)
{
	if (cmd[*i] && !(*double_quote) && cmd[*i] == '\'')
	{
		*i += 1;
		while (cmd[*i] && cmd[*i] != '\'')
			*i += 1;
	}
	if (cmd[*i] && !(*double_quote) && cmd[*i] == '"')
		*double_quote = 1;
	else if (cmd[*i] && *double_quote && cmd[*i] == '"')
		*double_quote = 0;
}


u_int8_t	check_eof_heredoc(char *input)
{
	if (!input)
	{
		if (g_status == 130)
			return (1);
		else
		{
			printf(RED EOF_HEREDOC RESET);
			return (1);
		}
	}
	return (0);
}

void	insert_var_str(char **str, int start)
{
	int		stop;
	char	*to_insert;
	char	*command;

	to_insert = NULL;
	command = *str;
	stop = stop_len(command, start); 
	if (command[start + 1] == '?')
		to_insert = ft_itoa(g_status);
	else if (ft_ischarset(command[start + 1], "\'\"_@#*-", ft_isalnum))
	{
		to_insert = get_to_insert(command, start, stop);
		if (!to_insert)
			return ;
	}
	if (ft_strinsert(&command, to_insert, start, stop))
		print_message("Allocation error.\n", RED, 1);
	*str = command;
}

void	heredoc_var_expand(int var_expand, char **input)
{
	int		i;
	int		double_quote;

	if (var_expand == 1)
	{
		while (flag(*input))
		{
			i = -1;
			double_quote = 0;
			while ((*input)[++i])
			{
				if ((*input)[i] == '$' && ft_ischarset((*input)[i + 1], "?\'\"_@#*-", ft_isalnum))
				{
					insert_var_str(input, i);
					break ;
				}
				jump_quotes(*input, &double_quote, &i);
			}
		}
	}
}

uint8_t	heredoc_loop(char *end_word, int var_expand, t_pipe_command *cmd)
{
	char	*input;

	input = "";
	remove_quotes_str(&end_word);
	input = garbage_addptr(readline("> "));
	if (check_eof_heredoc(input))
		return (1);
	if (ft_strequ(input, end_word))
			return (0);
	heredoc_var_expand(var_expand, &input);
	while (!ft_strequ(input, end_word))
	{
		write(cmd->fd_tmp, input, ft_strlen(input));
		write(cmd->fd_tmp, "\n", 1);
		input = garbage_addptr(readline("> "));
		if (check_eof_heredoc(input))
			return (1);
		if (ft_strequ(input, end_word))
			break ;
		heredoc_var_expand(var_expand, &input);
	}
	return (0);
}

uint8_t	write_in_tmp_file(t_pipe_command *cmd, int i)
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
	heredoc_loop(end_word, var_expand, cmd);
	g_status = save_status;
	close(cmd->fd_tmp);
	return (0);
}

uint8_t	heredoc_exec(t_pipe_command *cmd, int i, char *file_nbr)
{
	char	*tmp_file;

	if (cmd->fd_tmp)
		close(cmd->fd_tmp);
	while (cmd->cmd_content[i] == ' ' || cmd->cmd_content[i] == '\t')
		i++;
	tmp_file = ft_strjoin("file_test/tmp_file", file_nbr);
	if (!tmp_file)
		return (print_message(strerror(errno), RED, MALLOC_ERR), 1);
	cmd->fd_tmp = open(tmp_file, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (cmd->fd_tmp == -1)
		return (errno_file_error("tmpfile", 0), 1);
	if (write_in_tmp_file(cmd, i))
		return (1);
	cmd->fd_tmp = open(tmp_file, O_RDONLY);
	if (cmd->fd_tmp == -1)
		return (errno_file_error("tmpfile", 0), 1);
	// read_fd(cmd->fd_tmp);
	unlink(tmp_file);
	return (0);
}

uint8_t find_heredoc(t_pipe_command *cmd, int file_nbr)
{
	int		i;
	char	*file_nbr_str;

	i = 0;
	file_nbr_str = ft_itoa(file_nbr);
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
			if (heredoc_exec(cmd, i, file_nbr_str))
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
