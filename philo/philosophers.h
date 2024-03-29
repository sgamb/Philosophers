/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgambari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 14:23:47 by sgambari          #+#    #+#             */
/*   Updated: 2024/01/24 16:50:34 by sgambari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# define TRUE 1
# define FALSE 0

// # define right(N) (N + 1) % g->number_of_philosophers

# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>

typedef struct s_global
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_times_each_philosopher_must_eat;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	struct timeval	simulation_time_start;
	void			*philos;
	pthread_t		starvation_tracker_thread_id;
	int				until;
}	t_global;

typedef struct s_philo
{
	pthread_t		id;
	struct timeval	when_am_i_die;
	int				meal_num;
	pthread_mutex_t	meal_num_mutex;
	int				num;
	int				until;
	t_global		*global_data;
	pthread_mutex_t	last_ate_mutex;
}	t_philo;

// time_functions.c
int				ft_time_less(struct timeval t1, struct timeval t2);
void			my_print(t_global *global, int who, char *action);
struct timeval	time_sum(struct timeval t, unsigned int td);

// init_functions.c
t_global		*ft_init_global_data(int argc, char **argv);
void			ft_init_forks(t_global *global);
void			ft_init_philosophers(t_philo *philos, t_global *global);

// main.c
void			ft_run_philosophers(t_philo *philos, t_global *global);
void			ft_track_meal_num(t_philo *philos, t_global *global);
void			ft_wait_philosophers(t_philo *philos, t_global *global);
void			*ft_track_starvation(void *data);
void			ft_set_until_false(t_philo *philosophers, t_global *global);
void			*philo_routine(void *philo_data);

// parsing_functions.c
int				ft_atoi(const char *str);
int				ft_validate_input(int argc, char **argv);

#endif
