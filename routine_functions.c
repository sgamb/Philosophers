/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgambari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 21:24:26 by sgambari          #+#    #+#             */
/*   Updated: 2024/01/16 21:54:15 by sgambari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

pthread_mutex_t	*right_fork(int philo_num, t_global *g)
{
	return (&g->forks[(philo_num + 1) % g->number_of_philosophers]);
}

void	ft_take_forks(t_philo *philo, t_global *g)
{
	pthread_mutex_lock(right_fork(philo->num, g));
	my_print(g, philo->num, "has taken fork");
	pthread_mutex_lock(&g->forks[philo->num]);
	my_print(g, philo->num, "has taken fork");
}

void	ft_eat(t_philo *philo, t_global *g)
{
	my_print(g, philo->num, "is eating");
	pthread_mutex_lock(&philo->last_ate_mutex);
	philo->when_am_i_die = time_sum(philo->when_am_i_die, g->time_to_die);
	pthread_mutex_unlock(&philo->last_ate_mutex);
	usleep(1000 * g->time_to_eat);
	philo->meal_num++;
	pthread_mutex_unlock(&g->forks[philo->num]);
	pthread_mutex_unlock(right_fork(philo->num, g));
}

void	*philo_routine(void *data)
{
	t_philo			*philo;
	t_global		*global;

	philo = (t_philo *)data;
	global = philo->global_data;
	if (global->number_of_philosophers == 1)
		return (NULL);
	if (philo->num % 2 == 0)
		usleep(1234);
	while (philo->until)
	{
		ft_take_forks(philo, global);
		if (!philo->until)
			return (NULL);
		ft_eat(philo, global);
		if (!philo->until)
			return (NULL);
		my_print(global, philo->num, "is sleeping");
		usleep(1000 * global->time_to_sleep);
		if (!philo->until)
			return (NULL);
		my_print(global, philo->num, "is thinking");
	}
	return (NULL);
}
