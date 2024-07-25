#include "philo.h"

t_table *table(void)
{
	static  t_table table;
	return &table;
}
size_t  ft_strlen(const char *str)
{
	size_t  i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void    error(char *msg)
{
	write(2, msg, ft_strlen(msg));
	exit(1);
}

long long	getcurrtime(void)
{
	struct timeval  tv;

	if (gettimeofday(&tv, NULL))
		return (error("Error: gettimeofday failed\n"), -1);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}
void	sleep_ms(long long ms)
{
	long long start;

	start = getcurrtime();
	while (getcurrtime() - start < ms)
		usleep(100);
}

void	print_status(t_philo *philo, char *status)
{
	pthread_mutex_lock(&table()->log_mutex);
	if (!table()->dead)
		printf("%lld %d %s\n", getcurrtime() - philo->start_time, philo->id, status);
	pthread_mutex_unlock(&table()->log_mutex);
}

void	sleep_state(t_philo *philo)
{
	print_status(philo, "is sleeping 💤");
	sleep_ms(table()->data->time_to_sleep);
}
void	think_state(t_philo *philo)
{
	print_status(philo, "is thinking 🤔");
}


void    *philo_routine(void *data)
{
	while (!table()->dead)
	{
		think_state(data);
		sleep_state(data);
		// eat_state(data);
	}
}

int	ft_atoi(const char *str)
{
	int		i;
	long	result;

	i = 0;
	result = 0;
	while (str[i] == ' ' || str[i] == '\n' || str[i] == '\t')
		i++;
	if (str[i] == '+' && str[i + 1] != '-')
		i++;
	if (str[i] == '-')
		error("Error: arguments cannot be set to negative values\n");
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			error("Error: only numbers are allowed\n");
		result = result * 10;
		result += str[i] - '0';
		i++;
	}
	if (result > 2147483647)
		error("Error: number is too big\n");
	return (result);
}

void    init_data(t_data *data ,int ac, char **av)
{
	memset(data, 0, sizeof(t_data));
	data->nb_of_philos = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	if (!data->nb_of_philos)
		error("Error: number of philosophers cannot be set 0\n");
	if (data->nb_of_philos > MAX_PHILOS)
		error("Error: too many philosophers\nTry with less than 200\n");
	if (!data->time_to_die || !data->time_to_eat || !data->time_to_sleep)
		error("Error: time arguments cannot be set to 0 ms\n");
	if (data->time_to_die < MIN_TIME || data->time_to_eat < MIN_TIME || data->time_to_sleep < MIN_TIME)
		error("Error: time is too short\nTry with more than 60ms\n");
	data->meals = -1;
	if (ac == 6)
		data->meals = ft_atoi(av[5]);
	table()->data = data;
}


void	join_threads(t_philo *philo, t_data *data)
{
	int i;

	i = 0;
	while (i < data->nb_of_philos)
	{
		if (pthread_join(philo[i].thread, NULL))
			error("Error: pthread_join failed\n");
		i++;
	}
}

void    init_philos(t_philo *philo, t_data *data)
{
	int i;
	
	i = 0;
	while (i < data->nb_of_philos)
	{
		philo[i].id = i + 1;
		philo[i].meals_eaten = 0;
		philo[i].last_meal = getcurrtime();
		philo[i].is_dead = false;
		philo[i].left_fork = &table()->forks[i];
		philo[i].right_fork = &table()->forks[(i + 1) % data->nb_of_philos];
		philo[i].start_time = getcurrtime();
		if (pthread_create(&philo[i].thread, NULL, philo_routine, &philo[i]))
			error("Error: pthread_create failed\n");
		i++;
	}
}
void	init_forks(t_fork *forks, t_data *data)
{
	int i;

	i = 0;
	while (i < data->nb_of_philos)
	{
		if (pthread_mutex_init(&forks[i].mutex, NULL))
			error("Error: pthread_mutex_init failed\n");
		forks[i].is_available = true;
		i++;
	}
}

void	init_table(t_data *data)
{
	init_forks(table()->forks, data);
	init_philos(table()->philos, data);
	if (pthread_mutex_init(&table()->log_mutex, NULL))
		error("Error: pthread_mutex_init failed\n");
	table()->dead = false;
}

void	*monitor_routine(void *data)
{
	t_table *_table;
	int 	i;
	t_philo *philos;

	philos = table()->philos;
	_table = table();
	while (!_table->dead)
	{
		i = 0;
		while (i < _table->data->nb_of_philos)
		{
			if (getcurrtime() - _table->philos[i].last_meal > _table->data->time_to_die)
			{
				pthread_mutex_lock(&_table->log_mutex);
				printf("%lld %d died\n", getcurrtime() - philos[i].start_time , _table->philos[i].id);
				_table->dead = true;
				pthread_mutex_unlock(&_table->log_mutex);
				break ;
			}
			i++;
		}
	}
	return NULL;
}

void	destroy_table(t_table *table)
{
	int i;

	i = 0;
	while (i < table->data->nb_of_philos)
	{
		pthread_mutex_destroy(&table->forks[i].mutex);
		i++;
	}
	pthread_mutex_destroy(&table->log_mutex);
}

int	main(int ac, char **av)
{
	t_data data;
	pthread_t monitor;

	if (ac != 5 && ac != 6)
		error(USAGE);
	init_data(&data, ac, av);
	init_table(&data);
	if (pthread_create(&monitor, NULL, monitor_routine, NULL))
		error("Error: pthread_create failed\n");
	join_threads(table()->philos, table()->data);
	pthread_join(monitor, NULL);
	destroy_table(table());

	// start_simulation(); 
	return 0;
}
