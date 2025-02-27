#ifndef MINISHELL_H
#define MINISHELL_H

# include "libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include <stdbool.h>
# include <stdlib.h>
# include <signal.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <limits.h>

# define SUCCESS 1
# define ERROR 0

# define PERMISSION_DENIED 126 //need to check all error codes
# define COMMAND_NOT_FOUND 127
# define ERROR_CODE_MALLOC 1
# define ERROR_CODE_GENERAL 7
# define ERROR_CODE_QUOTES 8
# define ERROR_CODE_AMBIGUOUS_REDIRECT 9
# define ERROR_CODE_INVALID_VAR_NAME 10
# define ERROR_CODE_NO_PATH 11

//extern volatile sig_atomic_t g_prompt_flag;


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
	//pid_t			last_command_pid;
}	t_data;

typedef struct s_cmd {
    char	**args;
	struct s_cmd	*next;
    char	*input_redirect;
    char	*output_redirect;
    char	*append_redirect;
    char	*heredoc_delimiter;
    //int		pipe_in;
    //int		pipe_out;
	t_data	*data; //delete
	
	//struct s_cmd	*prev;
} t_cmd;

//delete after
void print_cmd_list(t_cmd *cmd_list, size_t count);
//void print_data(t_data *data);

char *find_executable(const char *cmd, char **paths);
char *get_path_from_env(char **env);
char **split_path(const char *path);

void free_cmd(t_cmd *cmd);
bool contains_special_char(t_token **tokens, t_token_type type);
int ft_str_only_spaces(const char *str);
//initialization
t_cmd	*init_structure(t_data *data, char **env);

// signals
void	signal_handler(void);

//tokenization
t_token **split_to_tokens(const char *str, t_data *data);
void	free_tokens(t_token **tokens);

//parsing
t_cmd	*parse_tokens(t_token **tokens);

// builtins
void	builtin_echo(t_cmd *cmd, t_data *data);
int	builtin_pwd(t_cmd *cmd, t_data *data);
int	builtin_export(t_cmd *cmd, t_data *data);
int	builtin_unset(t_cmd *cmd, t_data *data);
int	builtin_env(t_data *data);
int	builtin_exit(t_cmd *cmd, t_data *data);
int	builtin_cd(t_cmd *cmd, t_data *data);
char	*get_env_value(char **env, const char *key);
int		find_env_var(char **env, const char *var);
int		is_option(const char *arg);
char	*skip_spaces(char *str);
long	ft_atol(const char *str);

//execution
void	execute(t_token **tokens, t_cmd *cmd, t_data *data, char **env);
int		execute_command(char *cmd, t_data *data, char **args, char **env);
void execute_pipeline(t_token **tokens, t_cmd *cmd, t_data *data, char **env);
void	execute_for_one(t_token **tokens, t_cmd *cmd, t_data *data, char **env);
//int		contains_special_char(t_cmd *cmd, char delimiter);
//int		execute_redirection(t_cmd *cmd, char **env);
int	execute_redirection(t_cmd *cmd, t_data *data, char **env);

//expantion
char *expand_variable(const char *str, int *j, t_data *data);

#endif