#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct	s_philo
{
	pthread_t		id;
	struct timeval	when_am_i_die;
	int				meal_num;
	int				num;
	int				until;
}	t_philo;

#define N 5
#define TIME_TO_DIE 6000
#define TIME_TO_EAT 800000
#define TIME_TO_SLEEP 800000
#define MEAL_NUM 5
#define TRUE 1
#define FALSE 0

pthread_mutex_t	g_print;
struct timeval	g_simulation_time_start;
pthread_mutex_t	g_forks[N];

void	*philo_routine(void *philo_data);
void	ft_init_forks(void);
void	ft_init_philosophers(t_philo *philos);
void	ft_run_philosophers(t_philo *philos);
void	ft_track_meal_num(t_philo *philos);
void	ft_wait_philosophers(t_philo *philos);
void	ft_track_starvation(t_philo *philos);
void	ft_set_until_false(t_philo *philosophers);

unsigned int	ft_get_time()
{
	struct timeval	tv;
	unsigned int	sec_deltha;
	int	usec_deltha;

	gettimeofday(&tv, NULL);
	sec_deltha = tv.tv_sec - g_simulation_time_start.tv_sec;
	usec_deltha = tv.tv_usec - g_simulation_time_start.tv_usec;
	return (sec_deltha * 1000 + usec_deltha / 1000);
}

void	my_print(int who, char *action)
{
	unsigned int	time_from_start;

	time_from_start = ft_get_time();
	printf("%u %d %s\n", time_from_start, who, action);
}

int main(void)
{
	int				i;
	t_philo			philosophers[N];

	pthread_mutex_init(&g_print, NULL);
	gettimeofday(&g_simulation_time_start, NULL);
	ft_init_forks();
	ft_init_philosophers(philosophers);
	ft_run_philosophers(philosophers);
	ft_track_meal_num(philosophers);
	// ft_track_starvation(philosophers);
	ft_set_until_false(philosophers);
	ft_wait_philosophers(philosophers);
	return (0);
}

void	ft_set_until_false(t_philo *philosophers)
{
	for (int i = 0; i < N; i++)
		philosophers[i].until = FALSE;
}

void	ft_track_starvation(t_philo *philos)
{
	int	i;
	int until;

	until = TRUE;
	while (until)
	{
		i = 0;
		while (until && i < N)
		{
			// if (philos[i++].when_am_i_die >= NULL)
				until = FALSE;
		}
	}
}


void	ft_init_forks(void)
{
	int	i;

	i = 0;
	while (i < N)
		pthread_mutex_init(&g_forks[i++], NULL);
}

struct timeval time_sum(struct timeval t, unsigned int td)
{
}

void	ft_init_philosophers(t_philo *philos)
{
	struct timeval	now;
	int				i;

	gettimeofday(&now, NULL);
	i = 0;
	while (i < N)
	{
		philos[i].num = i;
		philos[i].when_am_i_die = time_sum(now, TIME_TO_DIE);
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
			if (philos[i++].meal_num >= MEAL_NUM)
				until = FALSE;
	}
}

void	ft_wait_philosophers(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < N)
		pthread_join(philos[i++].id, NULL);
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
		if (data->num % 2 == 0)
		{
			pthread_mutex_lock(&g_forks[data->num]); // take left
			my_print(data->num, "has taken left fork");
			pthread_mutex_lock(&g_forks[(data->num + 1) % N]); // take right
			my_print(data->num, "has taken right fork");
		}
		else
		{
			pthread_mutex_lock(&g_forks[(data->num + 1) % N]); // take right
			my_print(data->num, "has taken right fork");
			pthread_mutex_lock(&g_forks[data->num]); // take left
			my_print(data->num, "has taken left fork");
		}
		my_print(data->num, "is eating");
		usleep(TIME_TO_EAT);
		pthread_mutex_unlock(&g_forks[data->num]);
		pthread_mutex_unlock(&g_forks[(data->num + 1) % N]);
		my_print(data->num, "is sleeping");
		usleep(TIME_TO_SLEEP);
		my_print(data->num, "is thinking");
		data->meal_num++;
	}
	return (NULL);
}
