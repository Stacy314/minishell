/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/01/25 16:09:32 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd *initialize_cmd(t_data *data)
{
	t_cmd *cmd;
	
    cmd = ft_calloc(sizeof(t_cmd), 1);
    if (!cmd)
	{
        perror("calloc");
        exit(EXIT_FAILURE);
    }
    cmd->args = NULL;
    cmd->input_redirect = NULL;
    cmd->output_redirect = NULL;
    cmd->append_redirect = NULL;
    cmd->heredoc_delimiter = NULL;
    cmd->pipe_in = -1;
    cmd->pipe_out = -1;
    cmd->data = data;
	return (cmd);
}

t_cmd *init_structure(t_data *data, char **env)
{
	t_cmd *cmd;
	
	data->env = env;
	//printf("env: %s\n", env[33]); //HOME
	data->exit_status = 0;
	cmd = initialize_cmd(data);
	data->cmd = cmd;
	return (cmd);
}