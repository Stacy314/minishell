#ifndef MINISHELL_H
#define MINISHELL_H

# include "libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include <stdbool.h>
# include <stdlib.h>
# include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>


// Global flag for prompt control
//volatile sig_atomic_t g_prompt_flag = 0; 

#  define MAX_COMMANDS 265 // 128

//# define _DEFAULT_SOURSE

//enum
typedef enum e_token_type {
    WORD,
    PIPE,
    REDIRECT_IN,
    REDIRECT_OUT,
    APPEND,
    HEREDOC,    
    END       
} t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	int				index;
}	t_token;

typedef struct s_data
{
    int 			some_value; //need to delete
	char			**env;
	int				exit_status;
	struct s_cmd	*cmd;
}	t_data;

typedef struct s_cmd {
    char	**args;
    char	*input_redirect;
    char	*output_redirect;
    char	*append_redirect;
    char	*heredoc_delimiter;
    int		pipe_in;
    int		pipe_out;
	t_data	*data;
} t_cmd;

//initialization
t_cmd *init_structure(t_data *data, char **env);
// signals
void	signal_handler(void);

//tokenization
t_token **split_to_tokens(const char *str);
void	free_tokens(t_token **tokens);

//parsing
t_cmd	*parse_tokens(t_token **tokens);

// builtins
void	builtin_echo(t_cmd *cmd, t_data *data);
void	builtin_pwd(t_cmd *cmd);
void	builtin_export(t_cmd *cmd, t_data *data);
void	builtin_unset(t_cmd *cmd, t_data *data);
void	builtin_env(t_data *data);
void	builtin_exit(t_cmd *cmd, t_data *data);
void	builtin_cd(t_cmd *cmd, t_data *data);
char	*get_env_value(char **env, const char *key);
int		find_env_var(char **env, const char *var);
int		is_option(const char *arg);
char	*skip_spaces(char *str);

//execution
int		execute_command(char *cmd, t_data data, char **args);
void	execute_pipeline(t_cmd **cmd, char **env);
void	execute_for_one(t_token **tokens, t_cmd *cmd, t_data *data);
void	execute_for_many(t_token **tokens, t_cmd *cmd);
int		contains_special_char(t_cmd *cmd, char delimiter);
int		execute_redirection(t_cmd *cmd, char **env);

#endif