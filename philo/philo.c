#include "philo.h"

t_table	*get_table(void)
{
	static t_table table;
	return (&table);
}

static int	destroy_table(void)
{
	int		i;
	t_table	*table;

	i = 0;
	table = get_table();
	while (i < table->data->nb_of_philos)
	{
		if (pthread_mutex_destroy(&table->forks[i]))
			return (error("Error: pthread_mutex_destroy failed\n"), -1);
		i++;
	}
	if (pthread_mutex_destroy(&table->dead_mutex))
		return (error("Error: pthread_mutex_destroy failed\n"), -1);
	if (pthread_mutex_destroy(&table->log_mutex))
		return (error("Error: pthread_mutex_destroy failed\n"), -1);
}

static int	start_simulation(void)
{
	int		i;
	t_table *table;

	i = 0;
	table = get_table();
	while (i < table->data->nb_of_philos)
	{
		if (pthread_create(&table->philos[i].thread, NULL, philo_routine, &table->philos[i]))
			return (error("Error: pthread_create failed %d\n"), -1);
		i++;
	}
	if (pthread_create(&table->monitor, NULL, monitor_routine, NULL))
		return (error("Error: pthread_create failed\n"), -1);
}
static int	join_threads(void)
{
	int 	i;
	t_table *table;

	i = 0;
	table = get_table();
	while (i < table->data->nb_of_philos)
	{
		if (pthread_join(table->philos[i].thread, NULL))
			return (error("Error: pthread_join failed\n"), -1);
		i++;
	}
	if (pthread_join(table->monitor, NULL))
		return (error("Error: pthread_join failed\n"), -1);
}

int	main(int ac, char **av)
{
	t_data data;

	if (ac != 5 && ac != 6)
		return (error(USAGE), EXIT_FAILURE);
	if (init_table(&data, ac, av))
		return (EXIT_FAILURE);
	if (start_simulation())
		return (EXIT_FAILURE);
	if (join_threads())
		return (EXIT_FAILURE);
	if (destroy_table())
		return (EXIT_FAILURE);
	return (0);
}