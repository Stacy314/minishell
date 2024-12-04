#ifndef MINISHELL_H
#define MINISHELL_H

# include "libft/libft.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <stdbool.h>

#define TOKEN_BUFFER_SIZE 64

char**	split_to_token(char *str);

#endif