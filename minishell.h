#ifndef MINISHELL_H
#define MINISHELL_H

# include "libft/libft.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

//#define TOKEN_BUFFER_SIZE 1024 //64

//перелічувальний тип (enum) 
typedef enum e_token_type {
    WORD,         // 0
    PIPE,         // 1
    REDIRECT_IN,  // 2
    REDIRECT_OUT, // 3
    APPEND,       // 4
    HEREDOC,      // 5
    END           // 6
} t_token_type;

typedef struct s_token
{
	char	*value;
	t_token_type type;
	int index;
}	t_token;

t_token	split_to_token(char *str);
int		validation(int argc, char **argv);
char *ft_strtok(char *str, const char *delim);
void free_tokens(t_token **tokens);

#endif