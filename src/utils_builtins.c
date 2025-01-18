/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/01/17 16:59:34 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int is_option(const char *arg)
{
    return (arg && ft_strncmp(arg, "-n", 2) == 0);
}

char *get_env_value(char **env, const char *key)
{
	int len;
	int i;
   
	len = strlen(key);
	i = 0;
    while (env[i])
	{
        if (strncmp(env[i], key, len) == 0 && env[i][len] == '=')
		{
            return env[i] + len + 1;
        }
		i++;
    }
    return (NULL);
}
int find_env_var(char **env, const char *var)
{
    int len = strlen(var);
    for (int i = 0; env[i]; i++) {
        if (strncmp(env[i], var, len) == 0 && env[i][len] == '=') {
            return i; // Індекс знайденої змінної
        }
    }
    return -1; // Не знайдено
}
