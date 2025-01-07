#include "../minishell.h"

char* skip_spaces(char *str)
{
    while (*str != '\0' && *str == ' ')
		str++;
	return (str);
}