/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/12 16:57:31 by apechkov         ###   ########.fr       */
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
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>

# define MALLOC_ERROR -1
# define SUCCESS 1
# define ERROR 0
# define SYNTAXIS_ERROR 2
# define PERMISSION_DENIED 126
# define COMMAND_NOT_FOUND 127
# define INPUT_STOP 0
# define INPUT_CONTINUE 1
# define INPUT_ERROR 2
# define INPUT_SUCCESS 3
# define HEREDOC_RAND_MIN 1000
# define HEREDOC_RAND_MAX 9999
# define HEREDOC_MAX 16

extern sig_atomic_t	g_signal_flag;

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	APPEND,
	HEREDOC,
	NOTHING
}					t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	int				index;
	bool			touch_quotes;
}					t_token;

typedef struct s_tokenizer_state
{
	int				i;
	int				j;
	int				k;
	int				index;
	int				inside_quotes;
	int				buffer_size;
	int				empty_quotes;
	char			*buffer;
	char			quote_type;
	t_token			**tokens;
}					t_tokenizer_state;

typedef struct s_cmd
{
	char			**args;
	struct s_cmd	*next;
	char			**input_redirects;
	char			**output_redirects;
	char			**append_redirects;
	char			**heredoc_delimiter;
	bool			*heredoc_touch_quotes;
	int				heredoc_fd;
	pid_t			*pipe_pids;
	int				pipe_fd[2];
}					t_cmd;

typedef struct s_expand_ctx
{
	char			*result;
	size_t			buffer_size;
	size_t			i;
	size_t			k;
}					t_expand_ctx;

typedef struct s_data
{
	char			**env;
	int				exit_status;
	char			*input;
	char			*pwd_p;
	int				shlvl;
	t_token			**tokens;
	t_cmd			*cmd;
	bool			is_child;
	int				heredoc_count;
	int				saved_stdin;
}					t_data;

typedef struct s_redirect_append
{
	int				size;
	char			**new_array;
	bool			*new_flags;
	bool			has_flags;
}					t_redirect_append;

// utils
void				write_error(const char *format, ...);
int					ft_strcmp(const char *s1, const char *s2);
int					check_permissions(char *cmd);
char				*find_executable(const char *cmd, char **paths);
char				*get_path_from_env(char **env);
char				**split_path(const char *path);
void				free_cmd(t_cmd *cmd);
bool				contains_special_char(t_token **tokens, t_token_type type);
int					ft_str_only_spaces(const char *str);
void				free_env_copy(char **env_copy, int i);
int					prepare_input(t_data *data);
int					read_line_and_check(t_data *data);
int					tokenize_and_parse(t_data *data);
int					is_delim(char c, const char *delim);
void				close_fd_main(t_data *data);

// free
void				free_array(char **arr);
void				free_env(char **env);
void				free_all(t_data *data, t_token **tokens, t_cmd *cmd);
void				free_main(t_data *data, t_token **tokens, t_cmd *cmd);
void				*cleanup_and_null(t_tokenizer_state *state);

// initialization
t_cmd				*init_cmd(void);
int					init_data(t_data *data, char **env);
int					init_state(t_tokenizer_state *state, t_token **tokens);
t_cmd				*init_new_cmd(void);
int					update_shlvl_line(char **line);
void				free_env_copy(char **env_copy, int i);
int					prepare_input(t_data *data);

// signals
void				set_signals_main(void);
void				parent_ignore_signals(void);
void				parent_restore_signals(void);
void				handle_sigint_child(int sig);
void				handle_sigquit_child(int sig);
void				set_signals_child(void);
void				set_signals_heredoc(void);

// tokenization
int					handle_expansion(t_tokenizer_state *state, const char *str,
						t_data *data);
int					handle_quotes_and_redirects(t_tokenizer_state *state,
						const char *str, t_data *data);
int					create_word_token(t_tokenizer_state *state);
int					is_redirect(char c);
int					is_quote(char c);
int					is_pipe(char c);
int					expand_buffer(t_tokenizer_state *state);
void				*append_char_to_buffer(t_tokenizer_state *state, char c);
t_token				*create_token(const char *value, t_token_type type,
						int index, bool touch_quotes);
t_token				**split_to_tokens(const char *str, t_data *data);
void				free_tokens(t_token **tokens);
int					handle_redirection_tok(t_tokenizer_state *state,
						const char *str, t_data *data);
void				skip_spaces(const char *str, t_tokenizer_state *state);
int					is_logical_operator(const char *str,
						t_tokenizer_state *state);
int					create_pipe_operator(const char *str,
						t_tokenizer_state *state);
int					handle_token_word(t_tokenizer_state *state, const char *str,
						t_data *data);
int					update_quote_state(t_tokenizer_state *state, char c);
int					flush_word_before_redirect(t_tokenizer_state *state);
int					add_redirect_token(t_tokenizer_state *state,
						const char *symbol, t_token_type type, int advance);
char				*get_exit_status_value(t_data *data, int *j);
char				*extract_var_name(const char *str, int *j);
char				*search_env_value(char *var_name, t_data *data);
int					heredoc_delimiter_valid(t_tokenizer_state *state,
						const char *str);
int					fill_heredoc_buffer(t_tokenizer_state *state,
						const char *str, bool *touch_quotes);
int					handle_heredoc_delimiter(t_tokenizer_state *state,
						const char *str);
int					check_quotes_and_redirects(t_tokenizer_state *state,
						const char *str, t_data *data);
int					check_expansion_result(int result, t_tokenizer_state *state,
						const char *str);
int					create_nothing_token(const char *str,
						t_tokenizer_state *state);
int					process_token_word(const char *str,
						t_tokenizer_state *state, t_data *data);
int					handle_pipe_or_redirect(const char *str,
						t_tokenizer_state *state, t_data *data);
int					add_to_buffer(t_tokenizer_state *state, const char *str);
int					process_expansion(t_tokenizer_state *state, const char *str,
						t_data *data);
int					process_quotes_and_redirects(t_tokenizer_state *state,
						const char *str, t_data *data);

// parsing
int					parse_redirects(t_cmd *cmd, t_token *token,
						t_token_type type);
char				**append_to_args(char **args, char *new_arg);
int					build_command_list(t_cmd **head, t_token **tokens,
						t_data *data, int *i);
int					fill_cmd(t_cmd *cmd, t_token **tokens, t_data *data,
						int *i);
int					handle_redirection_parser(t_cmd *cmd, t_token **tokens,
						t_data *data, int *i);
int					is_redirect_token(t_token *token);
int					check_initial_syntax_errors(t_token **tokens, t_data *data);
char				***get_redirect_target(t_cmd *cmd, t_token_type type);
t_cmd				*parse_tokens(t_token **tokens, t_data *data);
int					init_append_data(t_redirect_append *data, char **redirects,
						bool **flags);
int					copy_existing_data(t_redirect_append *data,
						char **redirects, bool *flags);
int					add_new_value(t_redirect_append *data, const char *value,
						bool flag);
char				***get_redirect_target(t_cmd *cmd, t_token_type type);
bool				**get_redirect_flag_target(t_cmd *cmd, t_token_type type);
int					append_redirect_value(char ***redirects, const char *value,
						bool **flags, bool flag);

// builtins
void				builtin_echo(t_cmd *cmd, t_data *data, int token_index);
int					builtin_pwd(t_cmd *cmd, t_data *data, int token_index);
int					builtin_export(t_cmd *cmd, t_data *data, int token_index);
int					builtin_unset(t_cmd *cmd, t_data *data, int token_index);
int					builtin_env(t_data *data, t_cmd *cmd, int token_index);
int					builtin_exit(t_cmd *cmd, t_data *data, t_token **tokens,
						int token_index);
int					builtin_cd(t_cmd *cmd, t_data *data, int token_index);
char				**set_env_value(char **envp, const char *key,
						const char *value);
char				*get_env_value(char **env, const char *key);
int					check_cd_path(const char *dest_path, t_data *data);
int					find_env_var(char **env, const char *var);
int					is_option(const char *arg);
int					is_valid_identifier(const char *arg);
bool				is_numeric(const char *str);
long				ft_atol(const char *str, int *error);
int					ft_isspace(int c);
int					count_args(char **args);
void				print_sorted_env(char **env);
void				add_or_update_export(char *key, t_data *data);

// execution
void				execute(t_token **tokens, t_cmd *cmd, t_data *data);
int					execute_command(char *cmd, t_data *data, char **args);
void				execute_pipeline(t_cmd *cmd, t_data *data);
void				execute_for_one(t_token **tokens, t_cmd *cmd, t_data *data);
int					execute_redirection(t_cmd *cmd, t_data *data,
						t_token **tokens);
void				execute_heredoc(t_cmd *cmd, t_data *data);
void				handle_input_redirect(t_data *data, t_cmd *cmd);
void				handle_output_redirect(t_data *data, t_cmd *cmd);
void				handle_append_redirect(t_data *data, t_cmd *cmd);
int					handle_heredoc(t_cmd *cmd, char *heredoc_delimiter,
						size_t size, t_data *data);
void				apply_redirections(t_cmd *cmd, t_data *data);
int					handle_heredoc_pipe(t_cmd *cmd, t_data *data);
bool				is_quoted(const char *str);
char				*unquote_delimiter(char *quoted);
char				*expand_heredoc(const char *line, t_data *data);
int					add_or_update_env(char *arg, t_data *data);
int					handle_all_heredocs(t_cmd *cmd, t_data *data);
void				close_fd(t_cmd *cmd);
int					count_commands(t_cmd *cmd);
void				apply_redirections_for_heredoc(t_cmd *cmd, t_data *data);
int					cleanup_heredoc_file(int tmp_fd, char *tmp_filename,
						t_data *data, int status);
int					process_heredoc_input(t_cmd *cmd, char *heredoc_delimiter,
						int tmp_fd, t_data *data);
void				write_to_heredoc(int tmp_fd, char *line, int expand,
						t_data *data);
int					find_delimiter_index(t_cmd *cmd, char *heredoc_delimiter);
void				handle_child_process(char *executable, char **args,
						t_data *data, char **paths);
void				handle_parent_status(int status, t_data *data);
t_expand_ctx		*init_ctx(t_expand_ctx *ctx, const char *line);
char				*get_env_heredoc(const char *var, t_data *data);
pid_t				execute_first_command(t_token **tokens, t_cmd *cmd,
						t_data *data);
pid_t				execute_middle_command(t_cmd *current, t_cmd *cmd,
						t_data *data, int new_pipe_fd[2]);
pid_t				execute_last_command(t_token **tokens, t_cmd *current,
						t_cmd *cmd, t_data *data);
int					ensure_buffer_capacity(t_expand_ctx *ctx,
						size_t required_size);
void				start_heredoc(t_cmd *cmd, const char *input, t_data *data);
void				update_underscore(t_data *data, char *value);
char				*find_last_value(t_token **tokens);

// expantion
char				*expand_variable(const char *str, int *j, t_data *data);

#endif
