#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct	s_philo
{
	pthread_t	id;
	int			when_am_i_die;
	int			meal_num;
	int			num;
	int			until;
}	t_philo;

void			*philo_routine(void *philo_data);
pthread_mutex_t	print;

#define N 5
#define TIME_TO_DIE 6000
#define TIME_TO_EAT 800000
#define TIME_TO_SLEEP 800000
#define MEAL_NUM 5
#define TRUE 1
#define FALSE 0

pthread_mutex_t	forks[N];

void	ft_init_forks(void);
void	ft_init_philosophers(t_philo *philos);
void	ft_run_philosophers(t_philo *philos);
void	ft_track_meal_num(t_philo *philos);
void	ft_wait_philosophers(t_philo *philos);

int main(void)
{
	int				i;
	t_philo			philosophers[N];
	struct timeval	tv;

	pthread_mutex_init(&print, NULL);
	gettimeofday(&tv, NULL);
	srandom(tv.tv_sec);
	ft_init_forks();
	ft_init_philosophers(philosophers);
	ft_run_philosophers(philosophers);
	ft_track_meal_num(philosophers);
	for (int i = 0; i < N; i++)
		philosophers[i].until = FALSE;
	ft_wait_philosophers(philosophers);
	return (0);
}

void	ft_init_forks(void)
{
	int	i;

	i = 0;
	while (i < N)
		pthread_mutex_init(&forks[i++], NULL);
}

void	ft_init_philosophers(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < N)
	{
		philos[i].num = i;
		philos[i].when_am_i_die = TIME_TO_DIE;
		philos[i].meal_num = 0;
		philos[i].until = TRUE;
		i++;
	}
}

void	ft_run_philosophers(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < N)
	{
		pthread_create(&philos[i].id, NULL, &philo_routine, &philos[i]);
		i++;
	}
}

void	ft_track_meal_num(t_philo *philos)
{
	int	i;
	int until;

	until = TRUE;
	while (until)
	{
		i = 0;
		while (until && i < N)
		{
			if (philos[i].meal_num >= MEAL_NUM)
				until = FALSE;
			i++;
		}
	}
}

void	ft_wait_philosophers(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < N)
		pthread_join(philos[i++].id, NULL);
}

// void	take_fork(philo, fork);
void	*philo_routine(void *philo_data)
{
	t_philo			*data;
	struct timeval	tv;
	int				sleep_time;

	data = (t_philo *)philo_data;
	sleep_time = (1 + (data->num % N)) * 100000;
	while (data->until)
	{
		if (data->num % 2 == 0)
		{
			pthread_mutex_lock(&forks[data->num]); // take left
			gettimeofday(&tv, NULL);
			printf("%ld.%04d %d has taken left fork %d\n", tv.tv_sec % 100, tv.tv_usec / 10000, data->num, data->num);
			pthread_mutex_lock(&forks[(data->num + 1) % N]); // take right
			gettimeofday(&tv, NULL);
			printf("%ld.%04d %d has taken right fork %d\n", tv.tv_sec % 100, tv.tv_usec / 10000, data->num, (data->num + 1));
		}
		else
		{
			pthread_mutex_lock(&forks[(data->num + 1) % N]); // take right
			gettimeofday(&tv, NULL);
			printf("%ld.%04d %d has taken right fork %d\n", tv.tv_sec % 100, tv.tv_usec % 10000, data->num, (data->num + 1) % N);
			pthread_mutex_lock(&forks[data->num]); // take left
			gettimeofday(&tv, NULL);
			printf("%ld.%04d %d has taken left fork %d\n", tv.tv_sec % 100, tv.tv_usec % 10000, data->num, data->num);
		}
		gettimeofday(&tv, NULL);
		printf("%ld.%04d %d is eating\n", tv.tv_sec % 100, tv.tv_usec % 10000, data->num);
		usleep(TIME_TO_EAT);
		pthread_mutex_unlock(&forks[data->num]);
		pthread_mutex_unlock(&forks[(data->num + 1) % N]);
		gettimeofday(&tv, NULL);
		printf("%ld.%04d %d is sleeping\n", tv.tv_sec % 100, tv.tv_usec % 10000, data->num);
		usleep(TIME_TO_SLEEP);
		gettimeofday(&tv, NULL);
		printf("%ld.%04d %d is thinking\n", tv.tv_sec % 100, tv.tv_usec % 10000, data->num);
		data->meal_num++;
	}
	return (NULL);
}
