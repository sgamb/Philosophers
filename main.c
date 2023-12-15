/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgambari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 14:24:02 by sgambari          #+#    #+#             */
/*   Updated: 2023/12/15 18:04:12 by tumolabs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_global	*ft_handle_input(int argc, char **argv)
{
	t_global	*global;

	if (argc < 5)
		return (NULL);
	global = (t_global *)malloc(sizeof(t_global));
	if (global == NULL)
		return (NULL);
	global->number_of_philosophers = atoi(argv[1]); // TODO: update atoi
	global->time_to_die = atoi(argv[2]);
	global->time_to_eat = atoi(argv[3]);
	global->time_to_sleep = atoi(argv[4]);
	if (argc == 6)
		global->number_of_times_each_philosopher_must_eat = atoi(argv[5]);
	return (global);
}

int main(int argc, char **argv)
{
	t_philo			*philosophers;
	t_global		*global_data;

	pthread_mutex_init(&g_print, NULL);
	global_data = ft_handle_input(argc, argv);
	if (global_data == NULL)
		exit(1); // TODO: handle error better
	ft_init_forks(global_data);
	ft_init_simulation_start(global_data);
	philosophers = (t_philo *)malloc(sizeof(t_philo) * global_data->number_of_philosophers);
	ft_init_philosophers(philosophers, global_data);
	ft_run_philosophers(philosophers, global_data);
	// ft_track_meal_num(philosophers, global_data); // TODO: add check for argc
	ft_track_starvation(philosophers, global_data);
	ft_set_until_false(philosophers, global_data);
	ft_wait_philosophers(philosophers, global_data);
	return (0);
}

void	ft_set_until_false(t_philo *philosophers, t_global *global)
{
	for (int i = 0; i < global->number_of_philosophers; i++)
		philosophers[i].until = FALSE;
}

void	ft_track_starvation(t_philo *philos, t_global *global)
{
	struct timeval	now;
	int				i;

	i = 0;
	while (TRUE)
	{
		gettimeofday(&now, NULL);
		if (i == global->number_of_philosophers)
			i = 0;
		if (ft_time_less(philos[i++].when_am_i_die, now)) 
		{
			my_print(global, --i, "is died");
			return ;
		}
	}
}


void	ft_init_forks(t_global *global)
{
	int	i;

	global->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * global->number_of_philosophers);
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
		philos[i].when_am_i_die = time_sum(global->simulation_time_start, global->time_to_die);
		philos[i].meal_num = 0;
		philos[i].until = TRUE;
		philos[i].global_data = global;
		i++;
	}
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

void	ft_track_meal_num(t_philo *philos, t_global *global)
{
	int	i;
	int until;

	until = TRUE;
	while (until)
	{
		i = 0;
		while (until && i < global->number_of_philosophers)
			if (philos[i++].meal_num >= global->number_of_times_each_philosopher_must_eat)
				until = FALSE;
	}
}

void	ft_wait_philosophers(t_philo *philos, t_global *global)
{
	int	i;

	i = 0;
	while (i < global->number_of_philosophers)
		pthread_join(philos[i++].id, NULL);
}

void	*philo_routine(void *data)
{
	t_philo			*philo;
	t_global		*global;

	philo = (t_philo *)data;
	global = philo->global_data;
	while (philo->until)
	{
		if (philo->num % 2 == 0)
		{
			pthread_mutex_lock(&global->forks[philo->num]); // take left
			if (!philo->until)
				return (NULL);
			my_print(global, philo->num, "has taken left fork");
			pthread_mutex_lock(&global->forks[(philo->num + 1) % global->number_of_philosophers]); // take right
			my_print(global, philo->num, "has taken right fork");
		}
		else
		{
			pthread_mutex_lock(&global->forks[(philo->num + 1) % global->number_of_philosophers]); // take right
			if (!philo->until)
				return (NULL);
			my_print(global, philo->num, "has taken right fork");
			pthread_mutex_lock(&global->forks[philo->num]); // take left
			my_print(global, philo->num, "has taken left fork");
		}
		if (!philo->until)
			return (NULL);
		my_print(global, philo->num, "is eating");
		philo->when_am_i_die = time_sum(philo->when_am_i_die, global->time_to_die);
		usleep(1000 * global->time_to_eat);
		pthread_mutex_unlock(&global->forks[philo->num]);
		pthread_mutex_unlock(&global->forks[(philo->num + 1) % global->number_of_philosophers]);
		if (!philo->until)
			return (NULL);
		my_print(global, philo->num, "is sleeping");
		usleep(1000 * global->time_to_sleep);
		if (!philo->until)
			return (NULL);
		my_print(global, philo->num, "is thinking");
		philo->meal_num++;
	}
	return (NULL);
}
