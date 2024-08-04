#include "philo_bonus.h"

t_table	*get_table(void)
{
	static t_table table;
	return (&table);
}


void	init_data(t_data *data, int ac, char **av)
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
		pop_error("Error: number of meals cannot be set to 0\n");
	if (!data->nb_of_philos)
		pop_error("Error: number of philosophers cannot be set 0\n");
	if (data->nb_of_philos > MAX_PHILOS)
		pop_error("Error: too many philosophers\n");
	if (!data->time_to_die || !data->time_to_eat || !data->time_to_sleep)
		pop_error("Error: time arguments cannot be set to 0 ms\n");
	if (data->time_to_die < MIN_TIME || data->time_to_eat < MIN_TIME || data->time_to_sleep < MIN_TIME)
		pop_error("Error: time is too short\n");
}


void	init_table(t_data *data, int ac, char **av)
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
		pop_error("Error: mutex init failed\n");
	if (pthread_mutex_init(&table->dead_mutex, NULL))
		pop_error("Error: mutex init failed\n");
	
}

int main(int ac, char **av)
{
	t_data	data;

	int a = true;
	if (ac != 5 && ac != 6)
		pop_error(USAGE);
	init_table(&data, ac, av);
	// start_simulation();
	return (0);
}