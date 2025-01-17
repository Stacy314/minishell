/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/01/17 13:37:05 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	initialize_data(t_data *data, char **env)
{
	data->env = env;
	//printf("env: %s\n", env[33]); //HOME
	data->exit_status = 0;
	data->cmd = NULL;
	return ;
}
t_cmd *initialize_cmd(t_data *data)
{
	t_cmd *cmd;
	
    cmd = malloc(sizeof(t_cmd));
    if (!cmd) {
        perror("malloc failed");
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

void init_structure(t_data *data, char **env)
{
	initialize_data(data, env);
	initialize_cmd(data);
}