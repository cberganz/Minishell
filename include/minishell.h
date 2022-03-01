/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 17:59:05 by rbicanic          #+#    #+#             */
/*   Updated: 2022/03/01 19:33:09 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <setjmp.h>
# include <errno.h>
# include <error.h>
# include <getopt.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdint.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include "../libft/include/libft.h"

/*
**	Colors definition
*/

# define RED     "\x1b[31;1m"
# define GREEN   "\x1b[32m"
# define YELLOW  "\x1b[33m"
# define BLUE    "\x1b[34m"
# define MAGENTA "\x1b[35m"
# define CYAN    "\x1b[36m"
# define RESET   "\x1b[0m"

/*
**	Defines
*/

# define DO_NOT_CLOSE_PROG 0

/*
**	Structures
*/

typedef struct s_pipe_command
{
	char	*infile_operator;
	char	*outfile_operator;
	char	fd_in;
	char	fd_out;
	char	*infile;
	char	*outfile;
	char	*cmd_content;
	char	**exec_args;
	pid_t	pid;
}	t_pipe_command;

typedef struct s_command
{
	char	*control_op;
	char	*command;
	t_list	*command_list;
}	t_command;

/*
**	Global variables
*/

extern int	g_status;

/*
**	Terminal / print functions
*/

void	print_start(void);
char	*create_prompt(void);
void	prompt_loop(char *envp[]);
uint8_t	pipe_is_open(char *str);
void	print_message(char *msg, char *color, int exit_status);
void	errno_file_error(char *file, int exit_status);

/*
**	Parsing
*/

t_list	*global_parsing(char *input);
void	remove_comments(char **input);
t_list	*parse_step1(char *input);
void	*single_pipe_parsing(t_list **command_list);
char	**ft_split_pipe(const char *s, char *sep);//test

void	command_parsing(t_list *command_list);
void	tilde_expansion(t_list *command_list);
void	variable_expansion(t_list *command_list);
void	split_args(t_list *command_list);
void	remove_quotes_list(t_list *command_list);

/*
**	Signals
*/

void	sig_handler(int sigcode);

/*
**	Execution
*/

void	exec_main(t_list *cmd_list, char *envp[]);
void	exec_bin(t_pipe_command *command, char *envp[]);
int		exec_builtin(t_pipe_command *command, char *envp[]);
void	forking(t_list *command_list, char *envp[]);
void	wait_children(t_list *command_list);
char	*get_path(char **exec_args);

/*
**	Exit
*/

void	free_and_exit(int exit_code);
void	child_error_exit(int status_error, char *file);
void	eof_exit(void);

/*
**	Error management
*/

# define PATH_CMD_ERR 127
# define PATH_FILE_ERR 127
# define RIGHT_FILE_ERR 126
# define EXEC_ERR 1
# define MALLOC_ERR 2
# define FD_ERR 1

# define ARGS_ERR_MSG "Error : Invalid number of arguments.\n"
# define QUOTES_ERR_MSG "Minishell: open quotes are not interpreted.\n"
# define OPEN_PIPE_ERR_MSG "Minishell: open pipe are not interpreted.\n"
# define WRONG_CHAR_ERR_MSG "Minishell: uninterpreted token \
present outside single quotes.\n"
# define NEAR_TOKEN_ERR_MSG "Minishell: syntax error near unexpected token"
# define UNEXPECTED_EOF "Minishell: syntax error: unexpected end of file.\n"

uint8_t	near_unexpected_token_error(char **input, char **shell_prompt);
uint8_t	open_quotes(char *input);
uint8_t	not_interpreted_characters(char *input);
uint8_t	print_first_check_error(char *msg, char *token, char **input,
			char **shell_prompt);
int		quote_len(char *sub_input, char quote);

/*
**	redirections
*/

int		cmd_redirection_management(t_list *list);
int		file_len(char *s);
char	*ft_filedup(char *s, int len_of_file);
void	remove_file(int	len_of_file, char *s);
void	*out_redirection_parsing(t_pipe_command *cmd, char *operator, int i);
void	*in_redirection_parsing(t_pipe_command *cmd, char *operator, int i);

#endif
