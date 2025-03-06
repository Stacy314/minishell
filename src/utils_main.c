/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallyam <mgallyam@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/06 16:01:29 by mgallyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// int	contains_special_char(t_token **tokens, char delimiter)
//{
//	int	i;

//	//printf("contains_special_char\n");
//	i = 0;
//	while (tokens[i])
//	{
//		if (ft_strchr(tokens[i]->value, delimiter))
//		{
//			return (1);
//		}
//		i++;
//	}
//	return (0);
//}
bool	contains_special_char(t_token **tokens, t_token_type type)
{
	int	i;

	i = 0;
	while (tokens[i]) // Iterate through token list
	{
		if (tokens[i]->type == type) // Found a special character
			return (true);
		i++;
	}
	return (false);
}

int	ft_str_only_spaces(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != ' ' && str[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}

// void free_cmd(t_cmd *cmd)
//{
//	//int i = 0;
//    if (!cmd)
//        return ;
//	//while (cmd)
//    //{
//		while (cmd->args)
//		{
//			free(cmd->args);
//			cmd->args++;
//		}
//		while (cmd->input_redirect)
//		{
//			free(cmd->input_redirect);
//			cmd->input_redirect++;
//		}
//		while (cmd->output_redirect)
//		{
//			free(cmd->output_redirect);
//			cmd->output_redirect++;
//		}
//		while (cmd->append_redirect)
//		{
//			free(cmd->append_redirect);
//			cmd->append_redirect++;
//		}
//		while(cmd->heredoc_delimiter)
//		{
//			free(cmd->heredoc_delimiter);
//			cmd->heredoc_delimiter++;
//		}
//	//cmd++;
//	//}
//    //free(cmd);
//}

void free_cmd(t_cmd *cmd)
{
    if (!cmd)
        return;

    if (cmd->args)
    {
        int i = 0;
        while (cmd->args[i])
            free(cmd->args[i++]);
        free(cmd->args);
    }

    if (cmd->input_redirects)
    {
        int i = 0;
        while (cmd->input_redirects[i])
            free(cmd->input_redirects[i++]);
        free(cmd->input_redirects);
    }

    if (cmd->output_redirects)
    {
        int i = 0;
        while (cmd->output_redirects[i])
            free(cmd->output_redirects[i++]);
        free(cmd->output_redirects);
    }

    if (cmd->append_redirects)
    {
        int i = 0;
        while (cmd->append_redirects[i])
            free(cmd->append_redirects[i++]);
        free(cmd->append_redirects);
    }

    if (cmd->heredoc_delimiter)
        free(cmd->heredoc_delimiter);

    free(cmd);
}


int	careful_exit(t_data data, t_cmd *cmd, int exit_status)
{
	(void)exit_status;
	free_cmd(cmd);
	free(data.env);
	// free(data.);
	exit(data.exit_status);
}

// void print_data(t_data *data) //need to del
//{
//	if (!data)
//	{
//		printf("t_data is NULL\n");
//		return ;
//	}
//	printf("\n=== t_data ===\n");
//	printf("Exit Status: %d\n", data->exit_status);
//	// Вивід змінних оточення (env)
//	//printf("Environment Variables:\n");
//	//for (int i = 0; data->env && data->env[i]; i++)
//	//    printf("  [%d] %s\n", i, data->env[i]);
//	printf("=================\n");
//}
