#include "philo_bonus.h"

void	init_philos(t_philo *philo, long long pnumber)
{
	int	i;

	i = 0;
	while (i < pnumber)
	{
		philo[i].id = i + 1;
		philo[i].meals_eaten = 0;
		philo[i].last_meal = getcurrtime();
		i++;
	}
}

void	init_table(t_data *data, int ac, char **av)
{
	t_table	*table;

	table = get_table();
	init_data(data, ac, av);
	table->data = data;
	sem_unlink("/forks");
	sem_unlink("/log");
	sem_unlink("/dead");
	sem_unlink("/last_meal");
	sem_unlink("/full");
	table->forks = sem_open("/forks", O_CREAT, 0644, data->nb_of_philos);
	table->log_sem = sem_open("/log", O_CREAT, 0644, 1);
	table->dead_sem = sem_open("/dead", O_CREAT, 0644, 1);
	table->last_meal_sem = sem_open("/last_meal", O_CREAT, 0644, 1);
	table->full_sem = sem_open("/full", O_CREAT, 0644, 1);
	if (!table->forks || !table->log_sem || !table->dead_sem ||
		!table->last_meal_sem || !table->full_sem)
		pop_error("Error : sem_open failed\n");
	table->pids = malloc(sizeof(pid_t) * data->nb_of_philos);
	if (!table->pids)
		pop_error("Error : malloc failed\n");
	init_philos(table->philos, data->nb_of_philos);
}

void	start_simulation(void)
{
	int		i;
	t_table	*table;

	i = 0;
	table = get_table();
	table->start_time = getcurrtime();
	while (i < table->data->nb_of_philos)
	{
		table->pids[i] = fork();
		if (table->pids[i] == -1)
			pop_error("fork failed\n");
		if (table->pids[i] == 0)
		{
			spawn_philo(&table->philos[i]);
			exit(EXIT_SUCCESS);
		}
		usleep(100);
		i++;
	}
}

void	end_simulation(void)
{
	int		i;
	t_table	*table;

	i = 0;
	table = get_table();
	while (true)
	{
		if (table->dead_sem->__align == 0)
			break;
		if (table->full_sem->__align == 0)
			break;
		usleep(1000);
	}
	while (i < table->data->nb_of_philos)
	{
		if (kill(table->pids[i], SIGKILL))
			pop_error("kill failed\n");
		i++;
	}
	sem_post(table->dead_sem);
}

int	main(int ac, char **av)
{
	t_data	data;

	if (ac != 5 && ac != 6)
		pop_error(USAGE);
	init_table(&data, ac, av);
	start_simulation();
	end_simulation();
	clean_table();
	return (EXIT_SUCCESS);
}
