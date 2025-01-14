#ifndef MINISHELL_H
#define MINISHELL_H

# include "libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include <stdbool.h>
# include <stdlib.h>
# include <signal.h>

// Global flag for prompt control
//volatile sig_atomic_t g_prompt_flag = 0; 

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
	char	**cmd; //cat, ls, grep? 
	//char	**path;
	//char	*input;
	//char	*output;
	//char	*append;
	//char	*heredoc;
	char	**arg;
	char	**option;
	char	**env;
	int		exit_status;
}	t_data;

typedef struct s_command {
    char **args;              // Аргументы команды (включая саму команду)
    char *input_redirect;     // Файл для ввода (<)
    char *output_redirect;    // Файл для вывода (>)
    char *append_redirect;    // Файл для добавления (>>)
    char *heredoc_delimiter;  // Делимитер для << (heredoc)
    int pipe_in;              // Входящий пайп
    int pipe_out;             // Исходящий пайп
} t_command;

void	init_structure(t_data *data, char **env);

t_token	**split_to_token(char *str);
int		validation(int argc, char **argv);
char	*ft_strtok(char *str, const char *delim);
void	free_tokens(t_token **tokens);
void	execute(t_token **tokens, t_data *g_data);

// signals
void	signal_handler(void);

// builtins
void	builtin_echo(t_data g_data);
void	builtin_cd(t_data g_data);
void	builtin_pwd(t_data g_data);
void	builtin_export(t_data g_data);
void	builtin_unset(t_data g_data);
void	builtin_env(t_data g_data);
void	builtin_exit(t_data g_data);

t_token **split_to_tokens(const char *str);

// utils_builtins
char* 	skip_spaces(char *str);

#endif