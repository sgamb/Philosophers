#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct	s_philo
{
	int	when_am_i_die;
	int	meal_num;
	int	num;
	int	until;
}	t_philo;

void			*philo_routine(void *philo_data);
pthread_mutex_t	print;

#define N 2
#define TIME_TO_DIE 1000
#define MEAL_NUM 5
#define TRUE 1
#define FALSE 0

pthread_mutex_t	forks[N];

int main(void)
{
	int				i;
	pthread_t		philo_ids[N];
	t_philo			philos[N];
	struct timeval	tv;

	pthread_mutex_init(&print, NULL);
	gettimeofday(&tv, NULL);
	srandom(tv.tv_sec);
	i = 0;
	for (int i = 0; i < N; i++)
		pthread_mutex_init(&forks[i], NULL);
	while (i < N)
	{
		philos[i].num = i;
		philos[i].when_am_i_die = TIME_TO_DIE;
		philos[i].meal_num = 0;
		philos[i].until = TRUE;
		i++;
	}
	for (int i = 0; i < N; i++)
		pthread_create(&philo_ids[i], NULL, &philo_routine, &philos[i]);
	int until = TRUE;
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
	for (int i = 0; i < N; i++)
		philos[i].until = FALSE;
	for (int i = 0; i < N; i++)
		pthread_join(philo_ids[i], NULL);
	return (0);
}

void	*philo_routine(void *philo_data)
{
	t_philo			*data;
	struct timeval	tv;
	int				sleep_time;

	data = (t_philo *)philo_data;
	sleep_time = (1 + (data->num % N)) * 100000;
	while (data->until)
	{
		// take first fork
		if (data->num == 0)
		{
			pthread_mutex_lock(&forks[data->num]);
			gettimeofday(&tv, NULL);
			printf("%ld.%04d %d has taken left fork %d\n", tv.tv_sec % 100, tv.tv_usec % 10000, data->num, data->num);
			pthread_mutex_lock(&forks[(data->num + 1) % N]);
			gettimeofday(&tv, NULL);
			printf("%ld.%04d %d has taken right fork %d\n", tv.tv_sec % 100, tv.tv_usec % 10000, data->num, (data->num + 1));
		}
		else
		{
			pthread_mutex_lock(&forks[(data->num + 1) % N]);
			gettimeofday(&tv, NULL);
			printf("%ld.%04d %d has taken right fork %d\n", tv.tv_sec % 100, tv.tv_usec % 10000, data->num, (data->num + 1) % N);
			pthread_mutex_lock(&forks[data->num]);
			gettimeofday(&tv, NULL);
			printf("%ld.%04d %d has taken left fork %d\n", tv.tv_sec % 100, tv.tv_usec % 10000, data->num, data->num);
		}
		gettimeofday(&tv, NULL);
		printf("%ld.%04d %d is eating\n", tv.tv_sec % 100, tv.tv_usec % 10000, data->num);
		usleep(sleep_time);
		pthread_mutex_unlock(&forks[data->num]);
		pthread_mutex_unlock(&forks[(data->num + 1) % N]);
		data->meal_num++;
	}
	return (NULL);
}
