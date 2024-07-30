#include "philo.h"

t_table	*get_table(void)
{
	static t_table table;
	return (&table);
}

static void	destroy_table(void)
{
	int		i;
	t_table	*table;

	i = 0;
	table = get_table();
	while (i < table->data->nb_of_philos)
	{
		if (pthread_mutex_destroy(&table->forks[i]))
			error("Error: pthread_mutex_destroy failed\n");
		i++;
	}
	if (pthread_mutex_destroy(&table->log_mutex))
		error("Error: pthread_mutex_destroy failed\n");
}

static void	start_simulation(void)
{
	int i;
	t_table *table;

	i = 0;
	table = get_table();
	while (i < table->data->nb_of_philos)
	{
		if (pthread_create(&table->philos[i].thread, NULL, philo_routine, &table->philos[i]))
			error("Error: pthread_create failed\n");
		i++;
	}
	if (pthread_create(&table->monitor, NULL, monitor_routine, NULL))
		error("Error: pthread_create failed\n");
}
static void	join_threads(void)
{
	t_table *table;
	int 	i;

	table = get_table();
	i = 0;
	while (i < table->data->nb_of_philos)
	{
		if (pthread_join(table->philos[i].thread, NULL))
			error("Error: pthread_join failed\n");
		i++;
	}
	if (pthread_join(table->monitor, NULL))
		error("Error: pthread_join failed\n");
	
}

int	main(int ac, char **av)
{
	t_data data;

	if (ac != 5 && ac != 6)
		error(USAGE);
	init_table(&data, ac, av);
	start_simulation();
	join_threads();
	destroy_table();
	return (0);
}