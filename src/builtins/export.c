/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/06 19:25:04 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void write_error(const char *format, ...)
{
    va_list ap;
	char *s;
	//int	d;
	//char c;
	
    va_start(ap, format);
    while (*format)
    {
        if (*format == '%' && *(format + 1) != '\0')
        {
            format++;
            if (*format == 's')
            {
                s = va_arg(ap, char *);
                if (s)
                    ft_putstr_fd(s, 2);
            }
            //else if (*format == 'd')
            //{
            //    d = va_arg(ap, int);
            //    ft_putnbr_fd(d, 2);
            //}
            //else if (*format == 'c')
            //{
            //    c = (char)va_arg(ap, int);
            //    ft_putchar_fd(c, 2);
            //}
            else
            {
                ft_putchar_fd('%', 2);
                ft_putchar_fd(*format, 2);
            }
        }
        else
            ft_putchar_fd(*format, 2);
        format++;
    }
    va_end(ap);
}


int	is_valid_identifier(const char *arg)
{
	int	i;

	i = 0;
	if (!arg || !arg[0])
		return (0);
	if (!isalpha(arg[0]) && arg[0] != '_')
		return (0);
	while (arg[i] && arg[i] != '=')
	{
		if (!isalnum(arg[i]) && arg[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

// export a="s -lsa"
//		l$a
// export a='"' (shouldn't crash)
//		$a
// export a='"'
//		echo $a

int	builtin_export(t_cmd *cmd, t_data *data)
{
	int		i;
	char	*arg;
	char	*equal_sign;
	int		var_index;
	int		env_size;
	char	**new_env;

	int j; // too much declarations
	if (!cmd->args[1])
	{
		i = 0;
		while (data->env[i])
		{
			printf("declare -x %s\n", data->env[i]); // sort to alfabet
			i++;
		}
		return (1);
	}
	i = 1;
	while (cmd->args[i])
	{
		arg = cmd->args[i];
		if (!is_valid_identifier(arg))
		{
			write_error("minishell: export: `%s': not a valid identifier\n",
				arg);
			data->exit_status = 1;
			return (1);
		}
		equal_sign = ft_strchr(arg, '=');
		if (!equal_sign)
		{
			// printf("minishell: export: `%s': not a valid identifier\n", arg);
			data->exit_status = 0;
			return (1);
		}
		else if (!equal_sign++)
			return (write_error("minishell: export: `%s': not a valid identifier",
					arg), data->exit_status = 1);
		var_index = find_env_var(data->env, arg);
		if (var_index != -1)
		{
			// free(data->env[var_index]);
			data->env[var_index] = ft_strdup(arg);
		}
		else
		{
			env_size = 0;
			while (data->env[env_size])
				env_size++;
			new_env = malloc((env_size + 2) * sizeof(char *));
			if (!new_env)
			{
				perror("malloc");
				return (1);
			}
			j = 0;
			while (j < env_size)
			{
				new_env[j] = data->env[j];
				j++;
			}
			new_env[env_size] = ft_strdup(arg);
			new_env[env_size + 1] = NULL;
			// free(data->env);
			data->env = new_env;
		}
		i++;
	}
	return (0);
}
