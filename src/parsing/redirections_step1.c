/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_step1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 20:02:39 by rbicanic          #+#    #+#             */
/*   Updated: 2022/03/22 22:25:31 by rbicanic         ###   ########.fr       */
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

// void	write_in_tmp_file(t_pipe_command *cmd)
// {

// }

void	*out_redirection_parsing(t_pipe_command *cmd, char *operator, int i, char ***envp)
{
	int		len_of_file;
	char	*ambigous_redirect;

	if (cmd->fd_redirection[FD_OUT] != 1)
		close(cmd->fd_redirection[FD_OUT]);
	cmd->outfile_operator = operator;
	while (cmd->cmd_content[i] == operator[0] || cmd->cmd_content[i] == ' ')
		ft_strcpy(&cmd->cmd_content[i], &cmd->cmd_content[i + 1]);
	len_of_file = file_len(&cmd->cmd_content[i]);
	cmd->outfile = ft_filedup(&cmd->cmd_content[i], len_of_file);
	if (cmd->outfile == NULL)
		return (print_message(strerror(errno), RED, MALLOC_ERR), NULL);
	redirection_var_expand(1, &cmd->outfile, envp, "?\'\"_@#*-");//test
	if (!cmd->outfile[0])
	{
		print_message("Minishell: ", RED, 0);
		ambigous_redirect = ft_filedup(&cmd->cmd_content[i], len_of_file);
		if (ambigous_redirect == NULL)
			return (print_message(strerror(errno), RED, MALLOC_ERR), NULL);
		print_message(ambigous_redirect, RED, 0);
		print_message(": Ambigous redirection\n", RED, 0);
		printf (RESET);
		cmd->redirection_error = 1;
	}
	remove_file(len_of_file, &cmd->cmd_content[i]);
	if (!cmd->redirection_error)
	{
		if (!access(cmd->outfile, F_OK) && access(cmd->outfile, W_OK) == -1)
		{
			cmd->redirection_error = 1;
			return (errno_file_error(cmd->outfile, 1), (void *)1);
		}
		if (!ft_strncmp(operator, ">>", 2))
			cmd->fd_redirection[FD_OUT] = open(cmd->outfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
		else if (!ft_strncmp(operator, ">", 1))
			cmd->fd_redirection[FD_OUT] = open(cmd->outfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if (cmd->fd_redirection[FD_OUT] == -1)
		{
			cmd->redirection_error = 1;
			errno_file_error(cmd->outfile, 1);
		}
	}
	return ((void *)1);
}

char	*generate_tmp_file_name(void)
{
	int		nbr;
	char	*nbr_str;
	char	*file_name;

	nbr = -2147483648;
	while (nbr < 2147483647)
	{
		nbr_str = ft_itoa(nbr, LOOP);
		if (nbr_str == NULL)
			return (print_message(MALLOC_ERR_MSG, RED, 1), NULL);
		file_name = ft_strjoin("/tmp/", nbr_str, LOOP);
		if (file_name == NULL)
			return (print_message(MALLOC_ERR_MSG, RED, 1), NULL);
		if (access(file_name, F_OK) == -1)
			return (file_name);
		nbr++;
	}
	return (NULL);
}



void	*in_redirection_parsing(t_pipe_command *cmd, char *operator, int i, char ***envp)
{
	int		len_of_file;
	char	*ambigous_redirect;
	char	*tmp_file;

	if (cmd->fd_redirection[FD_IN] != 0)
		close(cmd->fd_redirection[FD_IN]);
	cmd->infile_operator = operator;
	while (cmd->cmd_content[i] == operator[0] || cmd->cmd_content[i] == ' ')
		ft_strcpy(&cmd->cmd_content[i], &cmd->cmd_content[i + 1]);
	len_of_file = file_len(&cmd->cmd_content[i]);
	if (ft_strncmp(operator, "<<", 2))
	{
		cmd->infile = ft_filedup(&cmd->cmd_content[i], len_of_file);
		if (cmd->infile == NULL)
			return (print_message(strerror(errno), RED, MALLOC_ERR), NULL);//exit si erreur dans process enfant 
		redirection_var_expand(1, &cmd->infile, envp, "?\'\"_@#*-");//test
		if (!cmd->infile[0])
		{
			print_message("Minishell: ", RED, 0);
			ambigous_redirect = ft_filedup(&cmd->cmd_content[i], len_of_file);
			if (ambigous_redirect == NULL)
				return (print_message(strerror(errno), RED, MALLOC_ERR), NULL);
			print_message(ambigous_redirect, RED, 0);
			print_message(": Ambigous redirection\n", RED, 0);
			printf(RESET); // printf ?
			cmd->redirection_error = 2;
		}
		if (!cmd->redirection_error)
		{
			if (access(cmd->infile, F_OK | W_OK) == -1)
			{
				cmd->redirection_error = 1;
				remove_file(len_of_file, &cmd->cmd_content[i]);
				return (errno_file_error(cmd->infile, 1), (void *)1);
			}
			cmd->fd_redirection[FD_IN] = open(cmd->infile, O_RDONLY);
			if (cmd->fd_redirection[FD_IN] == -1)
			{
				cmd->redirection_error = 1;
				remove_file(len_of_file, &cmd->cmd_content[i]);
				return (errno_file_error(cmd->infile, 1), (void *)1);
			}
		}
	}
	else if (!cmd->redirection_error && !ft_strstr(cmd->cmd_content, "<<") && !ft_strchr(cmd->cmd_content, '<') && cmd->heredoc_str)
	{
		tmp_file = generate_tmp_file_name();// secure + gestion de l'erreur peut etre donner un prompt
		if (!tmp_file)
			return (errno_file_error("tmpfile", 0), NULL);
		cmd->fd_redirection[FD_IN] = open(tmp_file, O_CREAT | O_WRONLY | O_TRUNC, 0777);
		if (cmd->fd_redirection[FD_IN] == -1)
			return (errno_file_error(tmp_file, 0), NULL);// pb sur ce retour
		write(cmd->fd_redirection[FD_IN], cmd->heredoc_str, ft_strlen(cmd->heredoc_str));
		close(cmd->fd_redirection[FD_IN]);
		cmd->fd_redirection[FD_IN] = open(tmp_file, O_RDONLY);
		if (cmd->fd_redirection[FD_IN] == -1)
			return (errno_file_error("tmpfile", 0), NULL);//pb sur ce retour
		unlink(tmp_file);
	}
	remove_file(len_of_file, &cmd->cmd_content[i]);
	return ((void *)1);
}
