/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgambari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 21:01:12 by sgambari          #+#    #+#             */
/*   Updated: 2024/01/16 21:01:13 by sgambari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_global	*ft_init_global_data(int argc, char **argv)
{
	t_global	*global;

	if (!ft_validate_input(argc, argv))
		return (NULL);
	global = (t_global *)malloc(sizeof(t_global));
	if (global == NULL)
		return (NULL);
	global->number_of_philosophers = ft_atoi(argv[1]);
	global->time_to_die = ft_atoi(argv[2]);
	global->time_to_eat = ft_atoi(argv[3]);
	global->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		global->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	return (global);
}

void	ft_init_forks(t_global *global)
{
	int	i;

	global->forks = (pthread_mutex_t *)malloc(
			sizeof(pthread_mutex_t) * global->number_of_philosophers);
	i = 0;
	while (i < global->number_of_philosophers)
		pthread_mutex_init(&global->forks[i++], NULL);
}

void	ft_init_philosophers(t_philo *philos, t_global *global)
{
	int	i;

	i = 0;
	while (i < global->number_of_philosophers)
	{
		philos[i].num = i;
		philos[i].when_am_i_die = time_sum(
				global->simulation_time_start, global->time_to_die);
		philos[i].meal_num = 0;
		philos[i].until = TRUE;
		philos[i].global_data = global;
		i++;
	}
}
