/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgambari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 14:35:32 by sgambari          #+#    #+#             */
/*   Updated: 2023/12/26 20:57:12 by serge            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_iswhitespace(int c)
{
	if (c == '\t' || c == '\n' || c == '\v'
		|| c == '\f' || c == '\r' || c == ' ')
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	int	result;

	while (ft_iswhitespace(*str))
		str++;
	result = 0;
	while (*str && ft_isdigit(*str))
	{
		result *= 10;
		result += (*str - '0');
		str++;
	}
	return (result);
}

int	ft_ispositive_int(char *str)
{
	while (*str)
		if (!ft_isdigit(*str++))
			return (FALSE);
	return (TRUE);
}

int	ft_validate_input(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return (FALSE);
	while (--argc)
		if (!ft_ispositive_int(argv[argc]))
			return (FALSE);
	return (TRUE);
}
