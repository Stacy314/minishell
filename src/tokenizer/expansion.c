/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/02 21:27:48 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//> $notexists echo "haha" (bash: $notexists: ambiguous redirect, exit code 1)

//$NOVAR (\n)

//echo hi >> "out" 

// echo hi >>">" out (write in > hi out)

char	*expand_variable(const char *str, int *j, t_data *data)
{
	char	var_name[256];
	int		k;
	char	**env;
	int		i;

	if (!str || !*str) // need to check
		return (NULL);
	k = 0;
	(*j)++;
	if (str[*j] == '?')
	{
		(*j)++;
		return (ft_itoa(data->exit_status));
	}
	while (str[*j] && (ft_isalnum(str[*j]) || str[*j] == '_'))
	{
		if (k < 255)
			var_name[k++] = str[*j];
		(*j)++;
	}
	var_name[k] = '\0';
	if (k == 0)
		return (ft_strdup("$"));
	env = data->env;
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], var_name, k) && env[i][k] == '=')
			return (ft_strdup(env[i] + k + 1));
		i++;
	}
	return (ft_strdup(""));
}
