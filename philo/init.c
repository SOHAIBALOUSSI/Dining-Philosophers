#include "philo.h"

static  int	ft_atoi(char *str)
{
	int		i;
	long long	result;

	i = 0;
	result = 0;
	while (str[i] == ' ' || str[i] == '\n' || str[i] == '\t')
		i++;
	if (str[i] == '+' && str[i + 1] != '-')
		i++;
	if (str[i] == '-')
		return (error("Error: arguments cannot be set to negative values\n"), -1);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (error("Error: only numbers are allowed\n"), -1);
		result = result * 10;
		result += str[i] - '0';
		if (result > INT_MAX)
			return (error("Error: number is too big\n"), -1);
		i++;
	}
	return ((int)result);
}

static int init_philos(t_philo *philo, t_data *data)
{
	int		i;
	t_table	*table;

	i = 0;
	table = get_table();
   	while (i < data->nb_of_philos)
	{
		philo[i].id = i + 1;
		philo[i].meals_eaten = 0;
		philo[i].last_meal = getcurrtime();
		philo[i].left_fork = &table->forks[i];
		philo[i].right_fork = &table->forks[(i + 1) % data->nb_of_philos];
		philo[i].is_dead = false;
		if (pthread_mutex_init(&philo[i].last_meal_mutex, NULL))
			return (error("Error: mutex init failed\n"), -1);
		if (pthread_mutex_init(&philo[i].meals_mutex, NULL))
			return (error("Error: mutex init failed\n"), -1);
		i++;
	}
	return (0);
}

static int	init_forks(pthread_mutex_t *forks, t_data *data)
{
	int i;

	i = 0;
	while (i < data->nb_of_philos)
	{
		if (pthread_mutex_init(&forks[i], NULL))
			return (error("Error: mutex init failed\n"), -1);
		i++;
	}
	return (0);
}

static int    init_data(t_data *data ,int ac, char **av)
{
	// memset(data, 0, sizeof(t_data));
	data->nb_of_philos = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	if (data->nb_of_philos == -1 || data->time_to_die == -1
		|| data->time_to_eat == -1 || data->time_to_sleep == -1)
		return (-1);
	data->meals = -1;
	if (ac == 6)
		data->meals = ft_atoi(av[5]);
	if (ac == 6 && data->meals == -1)
		return (-1);
	if (data->meals == 0)
		return (error("Error: number of meals cannot be set to 0\n"), -1);
	if (!data->nb_of_philos)
		return (error("Error: number of philosophers cannot be set 0\n"), -1);
	if (data->nb_of_philos > MAX_PHILOS)
		return (error("Error: too many philosophers\n"), -1);
	if (!data->time_to_die || !data->time_to_eat || !data->time_to_sleep)
		return (error("Error: time arguments cannot be set to 0 ms\n"), -1);
	if (data->time_to_die < MIN_TIME || data->time_to_eat < MIN_TIME
		|| data->time_to_sleep < MIN_TIME)
		return (error("Error: time is too short\n"), -1);
	return (0);
}
int init_table(t_data *data, int ac, char **av)
{
	int		i;
	t_table	*table;

	i = 0;
	table = get_table();
	if (init_data(data, ac, av) == -1)
		return (-1);
	table->data = data;
	table->dead = false;
	table->start_time = getcurrtime();
	if (pthread_mutex_init(&table->log_mutex, NULL))
		return (error("Error: mutex init failed\n"), -1);
	if (pthread_mutex_init(&table->dead_mutex, NULL))
		return (error("Error: mutex init failed\n"), -1);
	if (init_forks(table->forks, data) == -1)
		return (-1);
	if (init_philos(table->philos, data) == -1)
		return (-1);
	return (0);
}
