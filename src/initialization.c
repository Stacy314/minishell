/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/24 14:37:45 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/// bin/env | grep "SHLVL" (2)

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
	t_cmd *cmd;
	
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

int	increment_shlvl(t_data *data) // need to fix (should be 2 in the beginnig)
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
			if (!shlvl_str)
				return (ERROR);
			new_shlvl = ft_strjoin("SHLVL=", shlvl_str);
			if (!new_shlvl)
				return (free(shlvl_str), ERROR);
			return (/*free(shlvl_str),*/ /* free(data->env[i]),*/
					data->env[i] = new_shlvl,
					SUCCESS);
		}
		i++;
	}
	return (data->env[i] = ft_strdup("SHLVL=1"), data->env[i + 1] = NULL,
		SUCCESS);
}

// static char	**copy_env(char **env)
//{
//	int		i;
//	int		j;
//	char	**env_copy;

//	i = 0;
//	while (env[i])
//		i++;
//	env_copy = ft_calloc(sizeof(char *) * (i + 1), 1);
//	if (!env_copy)
//		return (NULL);
//	env_copy[i] = NULL;
//	j = 0;
//	while (j < i)
//	{
//		env_copy[j] = ft_strdup(env[j]);
//		if (!env_copy[j])
//		{
//			free_env(env_copy);
//			return (NULL);
//		}
//		j++;
//	}
//	return (env_copy);
//}
int	init_data(t_data *data, char **env)
{
	int	shlvl;

	data->env = env;
	// data->env = copy_env(env);
	// if (!data->env)
	//	return (ERROR);
	data->export_env = data->env;
	shlvl = increment_shlvl(data);
	if (!shlvl)
	{
		perror("init");
		return (ERROR);
	}
	data->exit_status = 0;
	data->input = NULL;
	data->pwd_p = NULL;
	return (SUCCESS);
}
