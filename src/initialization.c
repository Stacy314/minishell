/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/01/30 17:01:47 by apechkov         ###   ########.fr       */
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
    /* cmd->pipe_in = -1;
    cmd->pipe_out = -1; */
    cmd->data = data;
	cmd->next = NULL;
	return (cmd);
}

void append_cmd(t_cmd **head, t_cmd *new_cmd)
{
    t_cmd *temp;

    if (!head || !new_cmd)
        return ;
    // If the list is empty, the new node becomes the head
    if (*head == NULL)
    {
        *head = new_cmd;
        return ;
    }
    // Otherwise, traverse to the last node
    temp = *head;
    while (temp->next)
        temp = temp->next;
    // Attach new_cmd at the end
    temp->next = new_cmd;
}
t_cmd *create_cmd(void)
{
    t_cmd *new_cmd;

    new_cmd = malloc(sizeof(t_cmd));
    if (!new_cmd)
        return NULL;
    new_cmd->args = NULL;
    new_cmd->input_redirect = NULL;
    new_cmd->output_redirect = NULL;
    new_cmd->append_redirect = NULL;
    new_cmd->heredoc_delimiter = NULL;
    /* new_cmd->pipe_in = 0;
    new_cmd->pipe_out = 0; */
    new_cmd->data = NULL;
    
    // Initialize next to NULL for a brand-new node
    new_cmd->next = NULL;

    return new_cmd;
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