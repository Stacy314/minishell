/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/04 14:15:43 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <errno.h> //
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdarg.h>
# include <stdbool.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>

# define SUCCESS 1
# define ERROR 0
# define SYNTAXIS_ERROR 2
# define PERMISSION_DENIED 126
# define COMMAND_NOT_FOUND 127

//# define ERROR_CODE_MALLOC 1
//# define ERROR_CODE_GENERAL 7
//# define ERROR_CODE_QUOTES 8
//# define ERROR_CODE_AMBIGUOUS_REDIRECT 9
//# define ERROR_CODE_INVALID_VAR_NAME 10
//# define ERROR_CODE_NO_PATH 11

# define HEREDOC_RAND_MIN 1000
# define HEREDOC_RAND_MAX 999999

extern volatile sig_atomic_t	g_signal_flag;

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
	NOTHING,
	END
}								t_token_type;

typedef struct s_token
{
	char						*value;
	//char						*var_name;
	t_token_type				type;
	int							index;
}								t_token;

typedef struct s_tokenizer_state
{
	int							i;
	int							j;
	int							k;
	int							index;
	int							inside_quotes;
	int							buffer_size;
	int							empty_quotes;
	char						*buffer;
	char						quote_type;
	t_token						**tokens;
}								t_tokenizer_state;
typedef struct s_cmd
{
	char						**args;
	struct s_cmd				*next;
	char						**input_redirects;
	char						**output_redirects;
	char						**append_redirects;
	char						**heredoc_delimiter;
	int							heredoc_fd;
	pid_t						*pipe_pids;
	int							pipe_fd[2];
}								t_cmd;

typedef struct s_data
{
	char						**env;
	int							exit_status;
	char						*input;
	char						*pwd_p;
	int							shlvl;
	t_token						**tokens;
	t_cmd						*cmd;
	bool						is_child;
}								t_data;




bool	is_quoted(const char *str);
char	*unquote_delimiter(char *quoted);
char	*expand_heredoc(const char *line, t_data *data);
// utils
void							write_error(const char *format, ...);
int								ft_strcmp(const char *s1, const char *s2);
int								check_permissions(char *cmd);
char							*find_executable(const char *cmd, char **paths);
char							*get_path_from_env(char **env);
char							**split_path(const char *path);
void							free_cmd(t_cmd *cmd);
bool							contains_special_char(t_token **tokens,
									t_token_type type);
int								ft_str_only_spaces(const char *str);

// free
void							free_array(char **arr);
void							free_env(char **env);
void							free_all(t_data *data, t_token **tokens,
									t_cmd *cmd);
void							free_main(t_data *data, t_token **tokens,
									t_cmd *cmd);
void							*cleanup_and_null(t_tokenizer_state *state);

// initialization
t_cmd							*init_cmd(void);
int								init_data(t_data *data, char **env);
int								init_state(t_tokenizer_state *state,
									t_token **tokens);
t_cmd							*init_new_cmd(void);

// signals
void							set_signals_main(void);
void							set_signals_heredoc(void);
void							set_signals_child(void);
void							parent_ignore_signals(void);
void							parent_restore_signals(void);
void							handle_sigint_child(int sig);
void							handle_sigquit_child(int sig);

// tokenization
int								handle_expansion(t_tokenizer_state *state,
									const char *str, t_data *data);
int								handle_quotes_and_redirects(t_tokenizer_state *state,
									const char *str);
int								create_word_token(t_tokenizer_state *state);
int								is_redirect(char c);
int								is_quote(char c);
int								is_pipe(char c);
int								expand_buffer(t_tokenizer_state *state);
void							*append_char_to_buffer(t_tokenizer_state *state,
									char c);
t_token							*create_token(const char *value,
									t_token_type type, int index);
t_token							**split_to_tokens(const char *str,
									t_data *data);
void							free_tokens(t_token **tokens);
int								handle_redirection_tok(t_tokenizer_state *state,
									const char *str);
void							skip_spaces(const char *str,
									t_tokenizer_state *state);
int								is_logical_operator(const char *str,
									t_tokenizer_state *state);
int								create_pipe_operator(const char *str,
									t_tokenizer_state *state);
int								handle_token_word(t_tokenizer_state *state,
									const char *str, t_data *data);
int								update_quote_state(t_tokenizer_state *state,
									char c);
int								flush_word_before_redirect(t_tokenizer_state *state);
int								add_redirect_token(t_tokenizer_state *state,
									const char *symbol, t_token_type type,
									int advance);

// parsing
int								parse_redirects(t_cmd *cmd, t_token *token,
									t_token_type type);
char							**append_to_args(char **args, char *new_arg);
int								build_command_list(t_cmd **head,
									t_token **tokens, t_data *data, int *i);
int								fill_cmd(t_cmd *cmd, t_token **tokens,
									t_data *data, int *i);
int								handle_redirection_parser(t_cmd *cmd, t_token **tokens,
									t_data *data, int *i);
int								is_redirect_token(t_token *token);
int								check_initial_syntax_errors(t_token **tokens,
									t_data *data);
char							***get_redirect_target(t_cmd *cmd,
									t_token_type type);
int								initialize_redirect_array(char ***redirects,
									const char *value);
int								append_redirect_value(char ***redirects,
									const char *value);
t_cmd							*parse_tokens(t_token **tokens, t_data *data);

// builtins
void							builtin_echo(t_cmd *cmd, t_data *data, int token_index);
int								builtin_pwd(t_cmd *cmd, t_data *data, int token_index);
int								builtin_export(t_cmd *cmd, t_data *data, int token_index);
int								builtin_unset(t_cmd *cmd, t_data *data, int token_index);
int								builtin_env(t_data *data, t_cmd *cmd, int token_index);
int								builtin_exit(t_cmd *cmd, t_data *data,
									t_token **tokens, int token_index);
int								builtin_cd(t_cmd *cmd, t_data *data, int token_index);
char							**set_env_value(char **envp, const char *key,
									const char *value);
char							*get_env_value(char **env, const char *key);
int								check_cd_path(const char *dest_path,
									t_data *data);
int								find_env_var(char **env, const char *var);
int								is_option(const char *arg);
int								is_valid_identifier(const char *arg);
bool							is_numeric(const char *str);
long							ft_atol(const char *str, int *error);
int								ft_isspace(int c);
int								count_args(char **args);
void							print_sorted_env(char **env);
void							add_or_update_export(char *key, t_data *data);

// execution
void							execute(t_token **tokens, t_cmd *cmd,
									t_data *data);
int								execute_command(char *cmd, t_data *data,
									char **args);
void							execute_pipeline(t_token **tokens, t_cmd *cmd,
									t_data *data);
void							execute_for_one(t_token **tokens, t_cmd *cmd,
									t_data *data);
int								execute_redirection(t_cmd *cmd, t_data *data,
									char **env, t_token **tokens);
void							execute_heredoc(t_cmd *cmd);
void							handle_input_redirect(t_cmd *cmd);
void							handle_output_redirect(t_cmd *cmd);
void							handle_append_redirect(t_cmd *cmd);
int								handle_heredoc(t_cmd *cmd, size_t size);
void							apply_redirections(t_cmd *cmd, t_data *data);


int								handle_heredoc_pipe(t_cmd *cmd);
bool							handle_all_heredocs(t_cmd *cmd, t_data *data);
// expantion
char							*expand_variable(const char *str, int *j,
									t_data *data);
#endif
