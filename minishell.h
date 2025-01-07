#ifndef MINISHELL_H
#define MINISHELL_H

# include "libft/libft.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

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
	char	**cmd;
	char	**env;
	int		exit_status;
}	t_data;

t_token	**split_to_token(char *str);
int		validation(int argc, char **argv);
char	*ft_strtok(char *str, const char *delim);
void	free_tokens(t_token **tokens);
void	execute(t_token **tokens, t_data *g_data);

// builtins
void	builtin_echo(t_data g_data);
void	builtin_cd(t_data g_data);
void	builtin_pwd(t_data g_data);
void	builtin_export(t_data g_data);
void	builtin_unset(t_data g_data);
void	builtin_env(t_data g_data);
void	builtin_exit(t_data g_data);

// utils_builtins
char* 	skip_spaces(char *str);

#endif