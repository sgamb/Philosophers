/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgambari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 14:43:12 by sgambari          #+#    #+#             */
/*   Updated: 2023/12/14 15:41:30 by sgambari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int		ft_time_less(struct timeval t1, struct timeval t2)
{
	if (t1.tv_sec < t2.tv_sec)
		return (TRUE);
	if (t1.tv_sec > t2.tv_sec)
		return (FALSE);
	if (t1.tv_usec < t2.tv_usec)
		return (TRUE);
	return (FALSE);
}

void	ft_init_simulation_start(t_global *global)
{
	gettimeofday(&global->simulation_time_start, NULL);
}

unsigned int	ft_get_time(t_global *global)
{
	struct timeval	tv;
	unsigned int	sec_deltha;
	int				usec_deltha;

	gettimeofday(&tv, NULL);
	sec_deltha = tv.tv_sec - global->simulation_time_start.tv_sec;
	usec_deltha = tv.tv_usec - global->simulation_time_start.tv_usec;
	return (sec_deltha * 1000 + usec_deltha / 1000);
}

void	my_print(t_global *global, int who, char *action)
{
	unsigned int	time_from_start;

	time_from_start = ft_get_time(global);
	printf("%u %d %s\n", time_from_start, who, action);
}

