#include "philo.h"

static  int	ft_atoi(const char *str)
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

static void init_philos(t_philo *philo, t_data *data)
{
	int i;
	
	i = 0;
   	while (i < data->nb_of_philos)
	{
		philo[i].id = i + 1;
		philo[i].meals_eaten = 0;
		philo[i].last_meal = 0;
		philo[i].left_fork = &get_table()->forks[i];
		philo[i].right_fork = &get_table()->forks[(i + 1) % data->nb_of_philos];
		philo[i].is_dead = false;
		if (pthread_mutex_init(&philo[i].last_meal_mutex, NULL))
			error("Error: mutex init failed\n");
		if (pthread_mutex_init(&philo[i].meals_mutex, NULL))
			error("Error: mutex init failed\n");
		i++;
	}
}

static void	init_forks(pthread_mutex_t *forks, t_data *data)
{
	int i;

	i = 0;
	while (i < data->nb_of_philos)
	{
		if (pthread_mutex_init(&forks[i], NULL))
			error("Error: mutex init failed\n");
		i++;
	}
}

static void    init_data(t_data *data ,int ac, char **av)
{
	memset(data, 0, sizeof(t_data));
	data->nb_of_philos = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->meals = -1;
	if (ac == 6)
		data->meals = ft_atoi(av[5]);
	if (data->meals == 0)
		error("Error: number of meals cannot be set to 0\n");
	if (!data->nb_of_philos)
		error("Error: number of philosophers cannot be set 0\n");
	if (data->nb_of_philos > MAX_PHILOS)
		error("Error: too many philosophers\nTry with less than 200\n");
	if (!data->time_to_die || !data->time_to_eat || !data->time_to_sleep)
		error("Error: time arguments cannot be set to 0 ms\n");
	if (data->time_to_die < MIN_TIME || data->time_to_eat < MIN_TIME || data->time_to_sleep < MIN_TIME)
		error("Error: time is too short\nTry with more than 60ms\n");

}
void init_table(t_data *data, int ac, char **av)
{
	int		i;
	t_table	*table;

	i = 0;
	table = get_table();
	init_data(data, ac, av);
	table->data = data;
	table->dead = false;
	table->start_time = getcurrtime();
	if (pthread_mutex_init(&table->log_mutex, NULL))
		error("Error: mutex init failed\n");
	if (pthread_mutex_init(&table->dead_mutex, NULL))
		error("Error: mutex init failed\n");
	init_forks(table->forks, data);
	init_philos(table->philos, data);
}
