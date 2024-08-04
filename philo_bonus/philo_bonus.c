#include "philo_bonus.h"

t_table	*get_table(void)
{
	static t_table table;
	return (&table);
}


int	init_data(t_data *data, int ac, char **av)
{
	memset(data, 0, sizeof(t_data));
	data->nb_of_philos = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	if (data->nb_of_philos < 0 || data->time_to_die < 0 ||
			data->time_to_eat < 0 || data->time_to_sleep < 0)
		return (-1);
	data->meals = -1;
	if (ac == 6)
		data->meals = ft_atoi(av[5]);
	if (data->meals != -1 && data->meals < 0)
		return (-1);
	if (data->meals == 0)
		return (pop_error("Error: number of meals cannot be set to 0\n"), -1);
	if (!data->nb_of_philos)
		return (pop_error("Error: number of philosophers cannot be set 0\n"), -1);
	if (data->nb_of_philos > MAX_PHILOS)
		return (pop_error("Error: too many philosophers\n"), -1);
	if (!data->time_to_die || !data->time_to_eat || !data->time_to_sleep)
		return (pop_error("Error: time arguments cannot be set to 0 ms\n"), -1);
	if (data->time_to_die < MIN_TIME || data->time_to_eat < MIN_TIME || data->time_to_sleep < MIN_TIME)
		return (pop_error("Error: time is too short\n"), -1);
	return (0);
}


int	init_table(t_data *data, int ac, char **av)
{
	int	i;
	t_table *table;

	i = 0;
	table = get_table();
	if (init_data(data, ac, av))
		return (-1);
	table->data = data;
	table->dead = false;
	table->start_time = getcurrtime();
	if (table->start_time == -1)
		return (pop_error("Error: gettimeofday failed\n"), -1);
	if (pthread_mutex_init(&table->log_mutex, NULL))
		return (pop_error("Error: mutex init failed\n"), -1);
	if (pthread_mutex_init(&table->dead_mutex, NULL))
		return (pop_error("Error: mutex init failed\n"), -1);

	return (0);
}

int main(int ac, char **av)
{
	t_data	data;

	if (ac != 5 && ac != 6)
		return (pop_error(USAGE), -1);
	if (init_table(&data, ac, av))
		return (-1);
	return (0);
}