/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/17 21:50:21 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*init_cmd(void)
{
	t_cmd	*cmd;

	cmd = ft_calloc(sizeof(t_cmd), 1);  //leak
	if (!cmd)
	{
		perror("calloc");
		return (NULL);
	}
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

	i = 0;
	while (data->env[i])
	{
		if (ft_strncmp(data->env[i], "SHLVL=", 6) == 0)
		{
			shlvl_value = ft_atoi(data->env[i] + 6);
			shlvl_value++;
			shlvl_str = ft_itoa(shlvl_value);
			if (!shlvl_str)
			{
				return (ERROR);
			}
			new_shlvl = ft_strjoin("SHLVL=", shlvl_str);
			if (!new_shlvl)
			{
				free(shlvl_str);
				return (ERROR);
			}
			free(shlvl_str);
			free(data->env[i]);
			data->env[i] = new_shlvl;
			return (SUCCESS);
		}
		i++;
	}
	data->env[i] = ft_strdup("SHLVL=1"); 
	data->env[i + 1] = NULL;
	return (SUCCESS);
}
static char	**copy_env(char **env)
{
	int		i;
	int		j;
	char	**env_copy;

	i = 0;
	while (env[i])
		i++;
	env_copy = ft_calloc(sizeof(char *) * (i + 1), 1);
	if (!env_copy) 
		return (NULL);
	env_copy[i] = NULL;
	j = 0;
	while (j < i)
	{
		env_copy[j] = ft_strdup(env[j]);
		if (!env_copy[j])
		{
			free_env(env_copy);
			// треба звільнити все, що вже скопійовано
			// handle error ...
			return (NULL);
		}
		j++;
	}
	return (env_copy);
}
int	init_data(t_data *data, char **env)
{
	int	shlvl;

	// data->env = env;
	data->env = copy_env(env);
	if (!data->env)
		return (ERROR);
	data->export_env = env; //need to check 
	data->exit_status = 0;
	data->input = NULL;
	shlvl = increment_shlvl(data);
	if (!shlvl)
	{
		perror("init");
		return (ERROR);
	}
	return (SUCCESS);
}
