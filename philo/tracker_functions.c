/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tracker_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgambari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 22:40:53 by sgambari          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/01/24 12:31:46 by serge            ###   ########.fr       */
=======
/*   Updated: 2024/01/24 16:51:47 by sgambari         ###   ########.fr       */
>>>>>>> Meal num mutex and free
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	ft_track_meal_num(t_philo *philos, t_global *global)
{
	int	i;

	while (global->until)
	{
		i = 0;
		while (until && i < global->number_of_philosophers)
		{
			pthread_mutex_lock(&philos[i].meal_num_mutex);
			if (philos[i].meal_num
				>= global->number_of_times_each_philosopher_must_eat)
				until = FALSE;
			pthread_mutex_unlock(&philos[i].meal_num_mutex);
			i++;
		}
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
			global->until = FALSE;
			pthread_mutex_unlock(&philos[i].last_ate_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&philos[i].last_ate_mutex);
		i++;
		i %= global->number_of_philosophers;
	}
	return (NULL);
}
