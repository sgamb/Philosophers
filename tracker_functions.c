/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tracker_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgambari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 22:40:53 by sgambari          #+#    #+#             */
/*   Updated: 2024/01/16 22:41:16 by sgambari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	ft_track_meal_num(t_philo *philos, t_global *global)
{
	int	i;
	int	until;

	until = TRUE;
	while (until)
	{
		i = 0;
		while (until && i < global->number_of_philosophers)
			if (philos[i++].meal_num
				>= global->number_of_times_each_philosopher_must_eat)
				until = FALSE;
	}
}

void	*ft_track_starvation(void *data)
{
	struct timeval	now;
	int				i;
	t_philo			*philos;
	t_global		*global;

	global = (t_global *) data;
	philos = (t_philo *) global->philos;
	i = 0;
	while (global->until)
	{
		gettimeofday(&now, NULL);
		pthread_mutex_lock(&philos[i].last_ate_mutex);
		if (ft_time_less(philos[i].when_am_i_die, now))
		{
			my_print(global, i, "is died");
			return (NULL);
		}
		pthread_mutex_unlock(&philos[i].last_ate_mutex);
		i++;
		i %= global->number_of_philosophers;
	}
	return (NULL);
}