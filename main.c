#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct	s_philo
{
	int	time_to_die;
	int	meal_num;
	int	num;
}	t_philo;

void	*philo_routine(void *philo_data);

#define N 5

int main(void)
{
	int			i;
	pthread_t	philo_ids[N];
	t_philo		philos[N];
	struct timeval tv;

	gettimeofday(&tv, NULL);
	srandom(tv.tv_sec);
	i = 0;
	while (i < N)
	{
		philos[i].num = i;
		i++;
	}
	i = 0;
	while (i < N)
	{
		pthread_create(&philo_ids[i], NULL, &philo_routine, &philos[i]);
		i++;
	} 
	i = 0;
	// usleep(1000);
	while (i < N)
	{
		pthread_join(philo_ids[i++], NULL);
		gettimeofday(&tv, NULL);
		printf("joined %dth thread\ttime is %d\n", i-1, tv.tv_usec / 1000);
	}
	return (0);
}

void	*philo_routine(void *philo_data)
{
	t_philo	*data;
	struct timeval tv;
	int	sleep_time;

	data = (t_philo *)philo_data;
	gettimeofday(&tv, NULL);
	sleep_time = (0 + data->num) % 5 * 2000;
	printf("started %d\tcurrent time is %d\tgoing to sleep for %d milliseconds\n", data->num, tv.tv_usec / 1000, sleep_time / 1000);
	usleep(sleep_time);
	gettimeofday(&tv, NULL);
	printf("ended %d\tcurrent time is %d\n", data->num, tv.tv_usec / 1000);
	return (NULL);
}
