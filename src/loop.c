/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 20:29:44 by rbicanic          #+#    #+#             */
/*   Updated: 2022/02/22 19:21:30 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//uint8_t	pipe_is_open(char *str)
//{
//	int	i;
//
//	if (!str)
//		return (0);
//	i = ft_strlen(str) - 1;
//	while (str[i] == ' ' && i >= 0)
//		i--;
//	if (str[i] == '|' || (str[i] == '&' && (i > 0 && str[i - 1] == '&')))
//		return (1);
//	return (0);
//}

//Test function
void	print_lists(t_list	*list)
{
	t_list	*first;
	t_list	*tmp;

	if (!list)
		return ;
	first = list;
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
			if (tmp->next)
				printf(GREEN "%s => " RESET, ((t_pipe_command *)tmp->content)->cmd_content);
			else
				printf(GREEN "%s" RESET, ((t_pipe_command *)tmp->content)->cmd_content);
			tmp = tmp->next;
		}
		printf("\n\n");
		list = list->next;
	}
}

int8_t	input_error(char **input, char **shell_prompt, char **save)
{
	char	*tmp;

	if (!*input)
	{
		if (*save[0])
		{
			printf(UNEXPECTED_EOF);
			g_status = 2;
		}
		eof_exit(*input);
	}
	tmp = *input;
	*input = ft_strtrim(*input, " ");
	mem_remove(tmp);
	if (ft_strequ(*input, ""))
		return (-1);
	*save = "";
	if (near_unexpected_token_error(input, shell_prompt))
		return (-1);
	return (0);
}

uint8_t	input_first_read(char **input, char **shell_prompt, char **save)
{
	*input = ft_strjoin(" ", garbage_addptr(readline(*shell_prompt)));
	if (g_status != 130)
		*input = ft_strjoin(*save, *input);
//	if (pipe_is_open(*input))
//	{
//		g_status = -10;
//		*save = *input;
//		*shell_prompt = "> ";
//		return (1);
//	}
	if (input_error(input, shell_prompt, save))
		return (1);
	*save = "";
	return (0);
}

void	prompt_loop(void)
{
	char	*shell_prompt;
	char	*input;
	char	*save;
	t_list	*cmd_list;

	shell_prompt = create_prompt();
	input = "";
	save = "";
	while (1)
	{
		if (input_first_read(&input, &shell_prompt, &save))
			continue ;
		cmd_list = global_parsing(input);
		if (!cmd_list)
			free_and_exit(MALLOC_ERR);
		print_lists(cmd_list);
		if (!ft_strequ(input, ""))
		{
			g_status = 0; // A supprimer quand l'exec sera faite
			add_history(input);
		}
		del_garbage();
		input = "";
		shell_prompt = create_prompt();
	}
}
