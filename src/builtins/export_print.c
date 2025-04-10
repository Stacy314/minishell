/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/10 19:07:11 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_valid_identifier(const char *arg)
{
	int	i;

	i = 0;
	if (!arg || !arg[0])
		return (0);
	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		return (0);
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

char	**copy_env(char **env, int size)
{
	char	**copy;
	int		i;

	i = 0;
	copy = ft_calloc(sizeof(char *) * (size + 1), 1);
	if (!copy)
		return (perror("calloc"), NULL);
	while (i < size)
	{
		copy[i] = env[i];
		i++;
	}
	copy[size] = NULL;
	return (copy);
}

static void	bubble_sort_env(char **copy, int size)
{
	char	*tmp;
	int		i;
	int		j;

	i = 0;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - 1 - i)
		{
			if (ft_strncmp(copy[j], copy[j + 1], ft_strlen(copy[j])) > 0)
			{
				tmp = copy[j];
				copy[j] = copy[j + 1];
				copy[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	print_env_line(const char *env_line)
{
	const char	*equal_sign;
	int			key_len;
	char		*key;
	const char	*value;

	if (ft_strncmp(env_line, "_=", 2) == 0)
		return ;
	equal_sign = ft_strchr(env_line, '=');
	if (!equal_sign)
	{
		printf("declare -x %s\n", env_line);
		return ;
	}
	key_len = (int)(equal_sign - env_line);
	key = ft_calloc(key_len + 1, 1);
	if (!key)
	{
		perror("calloc");
		return ;
	}
	ft_strlcpy(key, env_line, key_len + 1);
	value = equal_sign + 1;
	printf("declare -x %s=\"%s\"\n", key, value);
	free(key);
}

void	print_sorted_env(char **env)
{
	int		size;
	char	**copy;
	int		i;

	size = 0;
	while (env[size])
		size++;
	copy = copy_env(env, size);
	if (!copy)
		return ;
	bubble_sort_env(copy, size);
	i = 0;
	while (i < size)
	{
		print_env_line(copy[i]);
		i++;
	}
	free(copy);
}
