#include "../minishell.h"
	
void free_tokens(t_token **tokens)
{
	int i;

	i = 0;
	if (!tokens)
        return;
   	while (tokens[i])
	{
        free(tokens[i]->value);
        free(tokens[i]);
		i++;
    }
    free(tokens);
}


