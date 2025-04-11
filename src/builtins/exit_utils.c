/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/11 20:14:27 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_isspace(int c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r')
		return (1);
	return (0);
}

static unsigned long	overflow_check(const char *str, int *index, int sign,
		int *error)
{
	unsigned long	result;

	result = 0;
	while (str[*index] && ft_isdigit((unsigned char)str[*index]))
	{
		if (sign == 1)
		{
			if (result > (unsigned long)LONG_MAX / 10
				|| (result == (unsigned long)LONG_MAX / 10 && (str[*index]
						- '0') > (int)(LONG_MAX % 10)))
				return (*error = 1, 0);
		}
		else
		{
			if (result > ((unsigned long)LONG_MAX + 1) / 10
				|| (result == ((unsigned long)LONG_MAX + 1) / 10 && (str[*index]
						- '0') > (int)(((unsigned long)LONG_MAX + 1) % 10)))
				return (*error = 1, 0);
		}
		result = result * 10 + (str[*index] - '0');
		(*index)++;
	}
	return (result);
}

static long	converting(const char *str, int *idx, int sign, int *error)
{
	unsigned long	result;

	result = overflow_check(str, idx, sign, error);
	if (*error == 1)
		return (0);
	while (ft_isspace((unsigned char)str[*idx]))
		(*idx)++;
	if (str[*idx] != '\0')
	{
		*error = 1;
		return (0);
	}
	if (sign == -1 && result == (unsigned long)LONG_MAX + 1)
		return (LONG_MIN);
	return ((long)result * sign);
}

long	ft_atol(const char *str, int *error)
{
	int		i;
	int		sign;
	long	res;

	*error = 0;
	i = 0;
	sign = 1;
	while (ft_isspace((unsigned char)str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (!str[i] || !ft_isdigit((unsigned char)str[i]))
		return (*error = 1, 0);
	if (*error == 1)
		return (0);
	res = converting(str, &i, sign, error);
	return (res);
}
