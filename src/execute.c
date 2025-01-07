#include "../minishell.h"

void	execute(t_token **tokens, t_data *g_data)
{
	int i;
	
	i = 0;
	while (tokens[i] != NULL)
	{
		if (tokens[i]->type == WORD)
		{
			if (ft_strncmp(tokens[i]->value, "echo", 4) == 0)
				builtin_echo(*g_data);
			if (ft_strncmp(tokens[i]->value, "cd", 2) == 0)
				builtin_cd(*g_data);
			if (ft_strncmp(tokens[i]->value, "pwd", 3) == 0)
				builtin_pwd(*g_data);
			if (ft_strncmp(tokens[i]->value, "export", 6) == 0)
				builtin_export(*g_data);
			if (ft_strncmp(tokens[i]->value, "unset", 5) == 0)
				builtin_unset(*g_data);
			if (ft_strncmp(tokens[i]->value, "env", 3) == 0)
				builtin_env(*g_data);
			if (ft_strncmp(tokens[i]->value, "exit", 4) == 0)
				builtin_exit(*g_data);
			else
				printf("minishell: command not found: %s", tokens[i]->value);
		}
		i++;
	}
}
