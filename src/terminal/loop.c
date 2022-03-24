/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 20:29:44 by rbicanic          #+#    #+#             */
/*   Updated: 2022/03/24 11:29:16 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

uint8_t	pipe_is_open(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = ft_strlen(str) - 1;
	while (str[i] == ' ' && i >= 0)
		i--;
	if (str[i] == '|' || (str[i] == '&' && (i > 0 && str[i - 1] == '&')))
		return (1);
	return (0);
}

int8_t	input_error(char **input, char **shell_prompt, char **envp[], int save_in)
{
	if (!*input)
	{
		close(save_in);
		eof_exit();
	}
	*input = ft_strtrim(*input, " ", LOOP);
	if (ft_strequ(*input, ""))
		return (-1);
	if (near_unexpected_token_error(input, shell_prompt, envp))
		return (-1);
	return (0);
}

uint8_t	input_first_read(char **input, char **shell_prompt, char **envp[], int save_in)
{
	int	save_status;

	save_status = g_status;
	g_status = -256;
	*input = garbage_addptr(readline(*shell_prompt), LOOP);
	if (g_status == 130)
		return (2);
	else if (g_status < -256)
		g_status = 130;
	else
		g_status = save_status;
	if (input_error(input, shell_prompt, envp, save_in))
		return (1);
	return (0);
}

//Test function
void	print_lists(t_list	*list)
{
	// t_list	*first;
	t_list	*tmp;

	if (!list)
		return ;
	// first = list;
	printf("\n");
	while (list)
	{
		if (((t_command *)list->content)->control_op)
			printf(CYAN "%s\n" RESET, (char *)((t_command *)list->content)->control_op);
		else
			printf(CYAN "(null)\n" RESET);
		printf("%s\n", (char *)((t_command *)list->content)->command);
		tmp = ((t_command *)list->content)->command_list;
		while (tmp)
		{
			printf(CYAN "\noutfile = %s" RESET, ((t_pipe_command *)tmp->content)->outfile);
			printf(CYAN "\ninfile = %s\n" RESET, ((t_pipe_command *)tmp->content)->infile);
			if (tmp->next)
				printf(GREEN "%s => " RESET, ((t_pipe_command *)tmp->content)->cmd_content);
			else
				printf(GREEN "%s" RESET, ((t_pipe_command *)tmp->content)->cmd_content);
			if (((t_pipe_command *)tmp->content)->exec_args)
			{
				while (*((t_pipe_command *)tmp->content)->exec_args)
				{
					printf(YELLOW "\n%s" RESET, *((t_pipe_command *)tmp->content)->exec_args);
					((t_pipe_command *)tmp->content)->exec_args++;
				}
			}
			tmp = tmp->next;
		}
		printf("\n\n");
		list = list->next;
	}
}

void	prompt_loop(char **envp[])
{
	char	*shell_prompt;
	char	*input;
	int		save_in;
	int		new_line;
	int		ret_first_read;
	t_list	*cmd_list;

	shell_prompt = create_prompt(envp);
	input = "";
	rl_outstream = stderr;
	new_line = 0;
	while (1)
	{
		if (new_line)
			ft_putendl_fd("", 2);
		signal(SIGINT, sig_handler);
		signal(SIGQUIT, sig_handler);
		signal(SIGTSTP, sig_handler);
		save_in = dup(STDIN_FILENO);// close ce save in
		new_line = 1;
		ret_first_read = input_first_read(&input, &shell_prompt, envp, save_in);
		if (ret_first_read == 1)
		{
			close(save_in);
			continue ;
		}
		if (ret_first_read == 2)
		{
			dup2(save_in, STDIN_FILENO);
			close(save_in);
			continue ;
		}
		cmd_list = global_parsing(input, save_in);
		if (!ft_strequ(input, ""))
			add_history(input);
		if (!cmd_list)
		{
			close(save_in);
			del_garbage(LOOP);
			input = "";
			shell_prompt = create_prompt(envp);
			continue ;
		}
		if (!exec_main(cmd_list, envp))
			new_line = 0;
		dup2(save_in, STDIN_FILENO);
		close(save_in);
		del_garbage(LOOP);
		input = "";
		shell_prompt = create_prompt(envp);
	}
}
