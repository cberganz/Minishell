/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 20:29:44 by rbicanic          #+#    #+#             */
/*   Updated: 2022/02/21 18:45:02 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

uint8_t	pipe_is_open(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = ft_strlen(str) - 1;
	while (str[i] == ' ' && i >= 0)// check si sur ' ' ou all spaces like \t
		i--;
	if (str[i] == '|' || (str[i] == '&' && (i > 0 && str[i - 1] == '&')))
		return (1);
	return (0);
}

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

int8_t	input_error(char **input, char **shell_prompt)
{
		char	*tmp;

		if (!*input)
			eof_exit(*input);//modif sur la fonction (input toujour null et check_open_pipe pas possible) => peut etre update error status auqnd open pipe
		tmp = *input;
		*input = ft_strtrim(*input, " ");
		mem_remove(tmp);
		if (ft_strequ(*input, ""))
			return (-1);
		if (near_unexpected_token_error(input, shell_prompt))
			return (-1);
		if (pipe_is_open(*input)) // si ctrl-d pendant l'affichage de la nouvelle ligne gérer la sortie du programme || si ctrl-c pas input pas reset + problem affichage ligne 
		{
			*shell_prompt = "> ";
			return (-1);
		}
		return (0);
}

void	prompt_loop(void)
{
	char	*shell_prompt;
	char	*input;
	t_list	*cmd_list;

	shell_prompt = create_prompt();
	input = "";
	while (1)
	{
		input = ft_strjoin(input, " ");
		if (g_status != 130)
		{
			input = ft_strjoin(input, garbage_addptr(readline(shell_prompt)));
		}
		else
		{
	//		rl_clear_line();
			input = garbage_addptr(readline(shell_prompt)); 
		}
		if (input_error(&input, &shell_prompt))
		{
			if (g_status == 130)
			{
				input = "";
				shell_prompt = create_prompt();
			}
			continue ;
		}
	//	if (g_status == 130)
	//	{
	//		continue ;
	//	}
		cmd_list = global_parsing(input);
		if (!cmd_list)
			free_and_exit(MALLOC_ERR);
		print_lists(cmd_list);
		if (!ft_strequ(input, ""))
		{
			g_status = 0;
			add_history(input);
		}
		del_garbage();
		input = "";
		shell_prompt = create_prompt();
	}
}
