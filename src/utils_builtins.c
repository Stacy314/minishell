/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/02/20 15:50:13 by apechkov         ###   ########.fr       */
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

int find_env_var(char **env, const char *var)
{
	int len;
	int i;
    
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
    long result = 0;
    int sign = 1;

    // Пропустити початкові пропуски
    while (isspace((unsigned char)*str))
	{
        str++;
    }
    // Обробити знак
	if (*str == '+')
	{
        str++;
    }
	else if (*str == '-')
	{
        sign = -1;
        str++;
    }
    // Парсити цифри
    while (isdigit((unsigned char)*str))
	{
        int digit = *str - '0';

        // Перевіряємо можливе переповнення перед множенням на 10 і додаванням цифри
		if (sign == 1) {
            // Якщо поточне result > (LONG_MAX - digit) / 10, наступна операція переповнить `long`
            if (result > (LONG_MAX - digit) / 10)
			{
                result = LONG_MAX;
                break;
            }
            result = result * 10 + digit;
        }
		else
		{
            // Для від'ємних значень перевіряємо (LONG_MIN + digit) / 10
            // Тут зручно порівнювати -result з використанням LONG_MAX, але уважно з дужками
            if (-result < (LONG_MIN + digit) / 10)
			{
                result = LONG_MIN;
                break;
            }
            result = result * 10 - digit;
        }
        str++;
    }
    return (result);
}