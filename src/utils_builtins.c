/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/02/22 18:16:36 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_option(const char *arg)
{
	return (arg && ft_strncmp(arg, "-n", 2) == 0);
}

char	*get_env_value(char **env, const char *key)
{
	int	len;
	int	i;

	len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

int	find_env_var(char **env, const char *var)
{
	int	len;
	int	i;

	len = ft_strlen(var);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var, len) == 0 && env[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

long	ft_atol(const char *str)
{
	long	result;
	int		sign;
	int		digit;

	result = 0;
	sign = 1;
	while (isspace((unsigned char)*str))
	{
		str++;
	}
	if (*str == '+')
	{
		str++;
	}
	else if (*str == '-')
	{
		sign = -1;
		str++;
	}
	while (isdigit((unsigned char)*str))
	{
		digit = *str - '0';
		if (sign == 1)
		{
			if (result > (LONG_MAX - digit) / 10)
			{
				result = LONG_MAX;
				break ;
			}
			result = result * 10 + digit;
		}
		else
		{
			if (-result < (LONG_MIN + digit) / 10)
			{
				result = LONG_MIN;
				break ;
			}
			result = result * 10 - digit;
		}
		str++;
	}
	return (result);
}
