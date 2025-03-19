/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallyam <mgallyam@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/19 14:52:28 by mgallyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdarg.h>
# include <stdbool.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>

# define SUCCESS 1
# define ERROR 0

# define PERMISSION_DENIED 126 // need to check all error codes
# define COMMAND_NOT_FOUND 127
//# define ERROR_CODE_MALLOC 1
//# define ERROR_CODE_GENERAL 7
//# define ERROR_CODE_QUOTES 8
//# define ERROR_CODE_AMBIGUOUS_REDIRECT 9
//# define ERROR_CODE_INVALID_VAR_NAME 10
//# define ERROR_CODE_NO_PATH 11

// extern volatile sig_atomic_t g_prompt_flag;

# define MAX_COMMANDS 265 // 128

extern pid_t		g_main_pid;

//# define _DEFAULT_SOURSE

// enum
typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	APPEND,
	HEREDOC,
	LOGICAL_AND,
	LOGICAL_OR,
	END
}					t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	int				index;
}					t_token;

typedef struct s_tokenizer_state
{
	int				i;
	int				j;
	int				k;
	int				index;
	int				inside_quotes;
	int				buffer_size;
	char			*buffer;
	char			quote_type;
	t_token			**tokens;
}					t_tokenizer_state;

typedef struct s_data
{
	int some_value; // need to delete
	char			**env;
	char			**export_env;
	int				exit_status;
	// struct s_cmd	*cmd;
	char *pwd; //
	// pid_t			last_command_pid;
	char			*input;
}					t_data;

typedef struct s_cmd
{
	char			**args;
	struct s_cmd	*next; // del
	char			**input_redirects;
	char			**output_redirects;
	char			**append_redirects;
	char			**heredoc_delimiter;
	// int		pipe_in;
	// int		pipe_out;
	//t_data *data; // delete

	// struct s_cmd	*prev;
}					t_cmd;


void free_env(char **env);
void				apply_redirections(t_cmd *cmd, t_data *data);

void				write_error(const char *format, ...);
void				free_array(char **arr);

int					check_permissions(char *cmd);

// delete after
void				print_cmd_list(t_cmd *cmd_list, size_t count);
// void print_data(t_data *data);
char				*find_executable(const char *cmd, char **paths);
char				*get_path_from_env(char **env);
char				**split_path(const char *path);
void				free_cmd(t_cmd *cmd);
bool				contains_special_char(t_token **tokens, t_token_type type);
int					ft_str_only_spaces(const char *str);

// initialization
void				initialize_state(t_tokenizer_state *state, t_token **tokens);
t_cmd				*init_cmd();
//t_data				*init_data(t_data *data, char **env);
int					init_data(t_data *data, char **env);

// signals
void				signal_handler(void);
void				set_child_signals(void);
void				set_heredoc_signals(void);

// tokenization
int					handle_expansion(t_tokenizer_state *state, const char *str, t_data *data);
int					handle_quotes_and_redirects(t_tokenizer_state *state, const char *str);
int					flush_buffer_to_token(t_tokenizer_state *state);
int					is_redirect(char c);
int					is_quote(char c);
int 				expand_buffer(t_tokenizer_state *state);
void 				*append_char_to_buffer(t_tokenizer_state *state, char c);
t_token				*create_token(const char *value, t_token_type type, int index);
t_token				**split_to_tokens(const char *str, t_data *data);
void				free_tokens(t_token **tokens);
int					handle_redirection(t_tokenizer_state *state, const char *str);
void				skip_spaces(const char *str, t_tokenizer_state *state);
int					is_logical_operator(const char *str, t_tokenizer_state *state);
int					is_pipe_operator(const char *str, t_tokenizer_state *state);
int					handle_token_word(t_tokenizer_state *state, const char *str, t_data *data);
void				*cleanup_and_null(t_token **tokens, t_tokenizer_state *state);
int					tokenize_loop(const char *str, t_tokenizer_state *state, t_token **tokens, t_data *data);
int					update_quote_state(t_tokenizer_state *state, char c);
int					flush_word_before_redirect(t_tokenizer_state *state);
int					add_redirect_token(t_tokenizer_state *state, const char *symbol, t_token_type type, int advance);

// parsing
t_cmd				*parse_tokens(t_token **tokens, t_data *data);
//void				parse_redirects(t_cmd *cmd, t_token *token,
//						t_token_type type);

// builtins
void				builtin_echo(t_cmd *cmd, t_data *data);
int					builtin_pwd(t_cmd *cmd, t_data *data);
int					builtin_export(t_cmd *cmd, t_data *data);
int					builtin_unset(t_cmd *cmd, t_data *data);
int					builtin_env(t_data *data, t_cmd *cmd);
int					builtin_exit(t_cmd *cmd, t_data *data);
int					builtin_cd(t_cmd *cmd, t_data *data);
char				**set_env_value(char **envp, const char *key,
						const char *value);
char				*get_env_value(char **env, const char *key);
int					check_cd_path(const char *dest_path, t_data *data);
int					find_env_var(char **env, const char *var);
int					is_option(const char *arg);
//char				*skip_spaces(char *str);
int					is_valid_identifier(const char *arg);
bool				is_numeric(const char *str);
long				ft_atol(const char *str, int *error);
int					ft_isspace(int c);
// char **set_env_value(char **envp, const char *key, const char *value);
int					count_args(char **args);
void				print_sorted_env(char **env);
void				add_or_update_export(char *key, t_data *data);

// execution
void				execute(t_token **tokens, t_cmd *cmd, t_data *data,
						char **env);
int					execute_command(char *cmd, t_data *data, char **args,
						char **env);
void				execute_pipeline(t_token **tokens, t_cmd *cmd, t_data *data,
						char **env);
void				execute_for_one(t_token **tokens, t_cmd *cmd, t_data *data,
						char **env);
int					execute_redirection(t_cmd *cmd, t_data *data, char **env);
void				handle_heredoc(t_cmd *cmd);

// expantion
char				*expand_variable(const char *str, int *j, t_data *data);
void				handle_input_redirect(t_cmd *cmd);

#endif
