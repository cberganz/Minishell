/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 20:29:44 by rbicanic          #+#    #+#             */
/*   Updated: 2022/03/22 22:17:50 by rbicanic         ###   ########.fr       */
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

int8_t	input_error(char **input, char **shell_prompt, char **envp[])
{
	char	*tmp; // a enlever ?

	if (!*input)
		eof_exit();
	tmp = *input; // a enlever ?
	*input = ft_strtrim(*input, " ", LOOP);
	mem_remove(tmp, LOOP); // a enlever ?
	if (ft_strequ(*input, ""))
		return (-1);
	if (near_unexpected_token_error(input, shell_prompt, envp))
		return (-1);
	return (0);
}

uint8_t	input_first_read(char **input, char **shell_prompt, char **envp[])
{
	int	save_status;

	save_status = g_status;
	g_status = -256;
	*input = garbage_addptr(readline(*shell_prompt), LOOP);
	if (g_status < -256)
		g_status = 130;
	else
		g_status = save_status;
	if (input_error(input, shell_prompt, envp))
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
	t_list	*cmd_list;

	shell_prompt = create_prompt(envp);
	input = "";
	rl_outstream = stderr;
	while (1)
	{
		signal(SIGINT, sig_handler);
		signal(SIGQUIT, sig_handler);
		signal(SIGTSTP, sig_handler);
		save_in = dup(STDIN_FILENO);// close ce save in
		if (input_first_read(&input, &shell_prompt, envp))
			continue ;
		cmd_list = global_parsing(input);
		if (!ft_strequ(input, ""))
			add_history(input);
		if (!cmd_list)
		{
			del_garbage(LOOP);
			input = "";
			shell_prompt = create_prompt(envp);
			continue ;
		}
		else if (exec_main(cmd_list, envp) == 1)
		{
			dup2(save_in, STDIN_FILENO);
			close(save_in);
			continue ;
		}
		// print_lists(cmd_list);
		close_heredoc_fds(cmd_list);
		del_garbage(LOOP);
		input = "";
		shell_prompt = create_prompt(envp);
	}
}
