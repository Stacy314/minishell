/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expantion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/02/17 17:33:08 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *replace_substring(char *str, char *to_replace, char *replacement)
{
    char *new_str;
    char *pos;
    int len;

    pos = ft_strnstr(str, to_replace, ft_strlen(str));  // Find the variable in the string
    if (!pos)
        return (str);

    len = ft_strlen(str) - ft_strlen(to_replace) + ft_strlen(replacement);
    new_str = ft_calloc(len + 1, sizeof(char));
    
    ft_strlcpy(new_str, str, pos - str);  // Copy before `$?`
    ft_strlcat(new_str, replacement, ft_strlen(new_str));  // Add replacement (exit_status)
    ft_strlcat(new_str, pos + ft_strlen(to_replace), ft_strlen(new_str));  // Copy after `$?`

    free(str);
    return (new_str);
}


char *expand_variables(char *str, t_data *data)
{
    char *expanded;
    char *exit_status_str;
    
    if (!str)
        return (NULL);
    
    expanded = ft_strdup(str);  // Duplicate input string

    if (ft_strnstr(expanded, "$?", ft_strlen(expanded)))  // Check for `$?`
    {
        exit_status_str = ft_itoa(data->exit_status);  // Convert exit_status to string
        expanded = replace_substring(expanded, "$?", exit_status_str);
        free(exit_status_str);
    }

    if (ft_strnstr(expanded, "$USER", ft_strlen(expanded)))
        expanded = replace_substring(expanded, "$USER", getenv("USER"));
    
    if (ft_strnstr(expanded, "$HOME", ft_strlen(expanded)))
        expanded = replace_substring(expanded, "$HOME", getenv("HOME"));

    return (expanded);
}
