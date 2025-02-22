/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expantion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/02/22 18:15:07 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//char *expand_variable(const char *str, int *j, t_data *data)
//{
//    char var_name[256];
//    int k = 0;
//    (*j)++;

//    while (str[*j] && (ft_isalnum(str[*j]) || str[*j] == '_'))
//    {
//        if (k < 255)
//            var_name[k++] = str[*j];
//        (*j)++;
//    }
//    var_name[k] = '\0';

//    if (k == 0)
//        return (ft_strdup("$"));

//    char **env = data->env;
//    int i = 0;
//    while (env[i])
//    {
//        if (!ft_strncmp(env[i], var_name, k) && env[i][k] == '=')
//            return (ft_strdup(env[i] + k + 1));
//        i++;
//    }
//    return (ft_strdup(""));
//}
char	*expand_variable(const char *str, int *j, t_data *data)
{
	char	var_name[256];
	int		k;
	char	**env;
	int		i;

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
