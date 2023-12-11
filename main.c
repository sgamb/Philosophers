#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct	s_global
{
	int	number_of_philosophers;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	number_of_times_each_philosopher_must_eat;
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
void	ft_init_philosophers(t_philo *philos, t_global *global);
void	ft_run_philosophers(t_philo *philos, t_global *global);
void	ft_track_meal_num(t_philo *philos, t_global *global);
void	ft_wait_philosophers(t_philo *philos, t_global *global);
void	ft_track_starvation(t_philo *philos, t_global *global);
void	ft_set_until_false(t_philo *philosophers);

unsigned int	ft_get_time()
{
	struct timeval	tv;
	unsigned int	sec_deltha;
	int				usec_deltha;

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
	if (argc == 6)
		global->number_of_times_each_philosopher_must_eat = atoi(argv[5]);
	return (global);
}

int main(int argc, char **argv)
{
	int				i;
	t_philo			philosophers[N];
	t_global		*global_data;

	pthread_mutex_init(&g_print, NULL);
	gettimeofday(&g_simulation_time_start, NULL);
	global_data = ft_handle_input(argc, argv);
	if (global_data == NULL)
		exit(1); // TODO: handle error better
	ft_init_forks();
	ft_init_philosophers(philosophers, global_data);
	ft_run_philosophers(philosophers, global_data);
	// ft_track_meal_num(philosophers, global_data); // TODO: add check for argc
	ft_track_starvation(philosophers, global_data);
	ft_set_until_false(philosophers);
	ft_wait_philosophers(philosophers, global_data);
	return (0);
}

void	ft_set_until_false(t_philo *philosophers)
{
	for (int i = 0; i < N; i++)
		philosophers[i].until = FALSE;
}

int		ft_time_less(struct timeval t1, struct timeval t2)
{
	if (t1.tv_sec < t2.tv_sec)
		return (TRUE);
	if (t1.tv_sec > t2.tv_sec)
		return (FALSE);
	if (t1.tv_usec < t2.tv_usec)
		return (TRUE);
	return (FALSE);
}

void	ft_track_starvation(t_philo *philos, t_global *global)
{
	struct timeval	now;
	int				i;
	int 			until;

	until = TRUE;
	while (until)
	{
		i = 0;
		while (until && i < global->number_of_philosophers)
		{
			gettimeofday(&now, NULL);
			if (ft_time_less(philos[i++].when_am_i_die, now)) 
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
	struct timeval	new_time;

	new_time.tv_sec = t.tv_sec + td / 1000;
	new_time.tv_usec = t.tv_usec + td % 1000;
	if (new_time.tv_usec > 1000000)
	{
		new_time.tv_sec++;
		new_time.tv_usec %= 1000000;
	}
	return (new_time);
}

void	ft_init_philosophers(t_philo *philos, t_global *global)
{
	struct timeval	now;
	int				i;

	gettimeofday(&now, NULL);
	i = 0;
	while (i < global->number_of_philosophers)
	{
		philos[i].num = i;
		philos[i].when_am_i_die = time_sum(now, global->time_to_die);
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
	t_philo			*philo_data;
	struct timeval	tv;
	int				sleep_time;
	t_global		*global_data;

	philo_data = (t_philo *)data;
	global_data = philo_data->global_data;
	sleep_time = (1 + (philo_data->num % global_data->number_of_philosophers)) * 100000;
	while (philo_data->until)
	{
		if (philo_data->num % 2 == 0)
		{
			pthread_mutex_lock(&g_forks[philo_data->num]); // take left
			my_print(philo_data->num, "has taken left fork");
			pthread_mutex_lock(&g_forks[(philo_data->num + 1) % global_data->number_of_philosophers]); // take right
			my_print(philo_data->num, "has taken right fork");
		}
		else
		{
			pthread_mutex_lock(&g_forks[(philo_data->num + 1) % global_data->number_of_philosophers]); // take right
			my_print(philo_data->num, "has taken right fork");
			pthread_mutex_lock(&g_forks[philo_data->num]); // take left
			my_print(philo_data->num, "has taken left fork");
		}
		my_print(philo_data->num, "is eating");
		usleep(TIME_TO_EAT);
		pthread_mutex_unlock(&g_forks[philo_data->num]);
		pthread_mutex_unlock(&g_forks[(philo_data->num + 1) % global_data->number_of_philosophers]);
		my_print(philo_data->num, "is sleeping");
		usleep(TIME_TO_SLEEP);
		my_print(philo_data->num, "is thinking");
		philo_data->meal_num++;
	}
	return (NULL);
}
