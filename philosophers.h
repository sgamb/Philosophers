/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgambari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 14:23:47 by sgambari          #+#    #+#             */
/*   Updated: 2023/12/14 15:46:16 by sgambari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# define TRUE 1
# define FALSE 0

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct	s_global
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_times_each_philosopher_must_eat;
	pthread_mutex_t	*forks;
	struct timeval	simulation_time_start;
}	t_global;

typedef struct	s_philo
{
	pthread_t		id;
	struct timeval	when_am_i_die;
	int				meal_num;
	int				num;
	int				until;
	t_global		*global_data;
}	t_philo;

// time_functions.c
int		ft_time_less(struct timeval t1, struct timeval t2);
unsigned int	ft_get_time();
void	my_print(t_global *global, int who, char *action);
void	ft_init_simulation_start(t_global *global);

void	*philo_routine(void *philo_data);
void	ft_init_forks(t_global *global);
void	ft_init_philosophers(t_philo *philos, t_global *global);
void	ft_run_philosophers(t_philo *philos, t_global *global);
void	ft_track_meal_num(t_philo *philos, t_global *global);
void	ft_wait_philosophers(t_philo *philos, t_global *global);
void	ft_track_starvation(t_philo *philos, t_global *global);
void	ft_set_until_false(t_philo *philosophers, t_global *global);

#endif
