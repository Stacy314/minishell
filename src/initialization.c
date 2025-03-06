/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/06 21:47:09 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*initialize_cmd(t_data *data)
{
	t_cmd	*cmd;

	cmd = ft_calloc(sizeof(t_cmd), 1);
	if (!cmd)
	{
		perror("calloc");
		exit(EXIT_FAILURE);
	}
	cmd->args = NULL;
	cmd->input_redirects = NULL;
	cmd->output_redirects = NULL;
	cmd->append_redirects = NULL;
	cmd->heredoc_delimiter = NULL;
	/* cmd->pipe_in = -1;
	cmd->pipe_out = -1; */
	cmd->data = data; // need to del
	cmd->next = NULL;
	return (cmd);
}

void	append_cmd(t_cmd **head, t_cmd *new_cmd)
{
	t_cmd	*temp;

	if (!head || !new_cmd)
		return ;
	if (*head == NULL)
	{
		*head = new_cmd;
		return ;
	}
	temp = *head;
	while (temp->next)
		temp = temp->next;
	temp->next = new_cmd;
}

t_cmd	*create_cmd(void)
{
	t_cmd	*new_cmd;

	new_cmd = ft_calloc(sizeof(t_cmd), 1);
	if (!new_cmd)
		return (NULL);
	new_cmd->args = NULL;
	new_cmd->input_redirects = NULL;
	new_cmd->output_redirects = NULL;
	new_cmd->append_redirects = NULL;
	new_cmd->heredoc_delimiter = NULL;
	/* new_cmd->pipe_in = 0;
	new_cmd->pipe_out = 0; */
	new_cmd->data = NULL;
	new_cmd->next = NULL;
	return (new_cmd);
}

void	increment_shlvl(t_data *data)
{
	int		i;
	int		shlvl_value;
	char	*new_shlvl;
	char	*shlvl_str;

	i = 0;
	while (data->env[i])
	{
		if (ft_strncmp(data->env[i], "SHLVL=", 6) == 0)
		{
			shlvl_value = ft_atoi(data->env[i] + 6);
			shlvl_value++;
			shlvl_str = ft_itoa(shlvl_value);
			new_shlvl = ft_strjoin("SHLVL=", shlvl_str);
			free(shlvl_str);
			// free(data->env[i]);
			data->env[i] = new_shlvl;
			return ;
		}
		i++;
	}
	data->env[i] = ft_strdup("SHLVL=1");
	data->env[i + 1] = NULL;
}

t_cmd	*init_structure(t_data *data, char **env)
{
	t_cmd	*cmd;

	data->env = env;
	data->exit_status = 0;
	increment_shlvl(data);
	cmd = initialize_cmd(data);
	data->cmd = cmd;
	return (cmd);
}
