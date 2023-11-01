#include <pthread.h>
#include <stdio.h>

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

	i = N;
	while (i--)
		pthread_create(&philo_ids[i], NULL, &philo_routine, &philos[i]);
	while (i < N)
		pthread_join(philo_ids[i++], NULL);
	return (0);
}

void	*philo_routine(void *philo_data)
{
	printf("I'm started\n");
	return (NULL);
}
