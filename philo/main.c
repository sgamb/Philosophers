/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgambari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 14:24:02 by sgambari          #+#    #+#             */
/*   Updated: 2024/01/16 22:39:33 by sgambari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int argc, char **argv)
{
	t_philo			*philosophers;
	t_global		*global_data;

	global_data = ft_init_global_data(argc, argv);
	if (global_data == NULL)
		return (1);
	ft_init_forks(global_data);
	gettimeofday(&global_data->simulation_time_start, NULL);
	philosophers = (t_philo *)malloc(sizeof(t_philo)
			* global_data->number_of_philosophers);
	ft_init_philosophers(philosophers, global_data);
	ft_run_philosophers(philosophers, global_data);
	pthread_create(&global_data->starvation_tracker_thread_id,
		NULL, &ft_track_starvation, global_data);
	ft_track_meal_num(philosophers, global_data);
	ft_set_until_false(philosophers, global_data);
	ft_wait_philosophers(philosophers, global_data);
	return (0);
}

void	ft_run_philosophers(t_philo *philos, t_global *global)
{
	int	i;

	i = 0;
	while (i < global->number_of_philosophers)
	{
		pthread_create(&philos[i].id, NULL, &philo_routine, &philos[i]);
		i++;
	}
}

void	ft_set_until_false(t_philo *philosophers, t_global *global)
{
	int	i;

	i = 0;
	while (i < global->number_of_philosophers)
		philosophers[i++].until = FALSE;
	global->until = FALSE;
}

void	ft_wait_philosophers(t_philo *philos, t_global *global)
{
	int	i;

	i = 0;
	while (i < global->number_of_philosophers)
		pthread_join(philos[i++].id, NULL);
	pthread_join(global->starvation_tracker_thread_id, NULL);
}
