/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/24 18:14:15 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <readline/chardefs.h>

// /bin/env | grep "SHLVL" (2)

int		g_shlvl = 1;

void	initialize_state(t_tokenizer_state *state, t_token **tokens)
{
	state->i = 0;
	state->j = 0;
	state->k = 0;
	state->index = 0;
	state->inside_quotes = 0;
	state->quote_type = 0;
	state->buffer_size = 64;
	state->buffer = ft_calloc(state->buffer_size, sizeof(char));
	if (!state->buffer)
	{
		perror("calloc");
		return ;
	}
	state->tokens = tokens;
}
t_cmd	*init_new_cmd(void)
{
	t_cmd	*cmd;

	cmd = ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (perror("calloc"), NULL);
	cmd->args = NULL;
	cmd->input_redirects = NULL;
	cmd->output_redirects = NULL;
	cmd->append_redirects = NULL;
	cmd->heredoc_delimiter = NULL;
	cmd->next = NULL;
	return (cmd);
}

int	increment_shlvl(t_data *data)
{
	int		i;
	int		shlvl_value;
	char	*new_shlvl;
	char	*shlvl_str;

	new_shlvl = NULL;
	i = 0;
	while (data->env[i])
	{
		if (ft_strncmp(data->env[i], "SHLVL=", 6) == 0)
		{
			shlvl_value = ft_atoi(data->env[i] + 6);
			shlvl_value++;
			shlvl_str = ft_itoa(shlvl_value);
			if (!shlvl_str)
				return (ERROR);
			new_shlvl = ft_strjoin("SHLVL=", shlvl_str);
			//free(shlvl_str);
			if (!new_shlvl)
				return (ERROR);
			data->env[i] = new_shlvl;
			return (SUCCESS);
		}
		i++;
	}
	return (data->env[i] = ft_strdup("SHLVL=1"), data->env[i + 1] = NULL,
		SUCCESS);
}

char	**copy_env(char **env)
{
	int		i;
	int		count;
	char	**new_env;

	i = 0;
	count = 0;
	while (env[count])
		count++;
	new_env = ft_calloc(sizeof(char *) * (count + 1), 1);
	if (!new_env)
		return (NULL);
	while (i < count)
	{
		new_env[i] = ft_strdup(env[i]);
		if (!new_env[i])
		{
			while (--i >= 0)
				free(new_env[i]);
			free(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

int	init_data(t_data *data, char **env)
{
	data->env = copy_env(env);
	if (!data->env)
	{
		perror("init");
		return (ERROR);
	}
	data->export_env = data->env;
	data->shlvl = increment_shlvl(data);
	if (!data->shlvl)
	{
		perror("init");
		return (ERROR);
	}
	data->exit_status = 0;
	data->input = NULL;
	data->pwd_p = NULL;
	return (SUCCESS);
}
