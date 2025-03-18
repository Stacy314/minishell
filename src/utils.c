/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/18 19:05:24 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	contains_special_char(t_token **tokens, t_token_type type)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		if (tokens[i]->type == type)
			return (true);
		i++;
	}
	return (false);
}

int	ft_str_only_spaces(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != ' ' && str[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}

static int	print_err(char specifier, va_list ap)
{
	if (specifier == 'c')
		return (ft_putchar_fd(va_arg(ap, int), 2));
	else if (specifier == 's')
		return (ft_putstr_fd(va_arg(ap, char *), 2));
	else if (specifier == '%')
		return (ft_putchar_fd('%', 2));
	else
	{
		ft_putchar_fd('%', 2);
		return (ft_putchar_fd(specifier, 2));
	}
}
void	write_error(const char *format, ...)
{
	va_list	ap;
	int		i;

	i = 0;
	va_start(ap, format);
	while (*format != '\0')
	{
		if (*format == '%')
		{
			i += print_err(*(++format), ap);
		}
		else
		{
			i += write(2, format, 1);
		}
		++format;
	}
	va_end(ap);
}
