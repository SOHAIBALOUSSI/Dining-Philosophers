#include "philo_bonus.h"
#include "string.h"

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

// void	init_forks(sem_t **forks, long long pnumber)
// {
// 	int i;

// 	i = 0;
// 	while (i < pnumber)
// 	{
// 		forks[i] = sem_open("fork_nb_", O_CREAT, NULL, 1);
// 		if (forks[i] == SEM_FAILED)
// 		{
// 			pop_error("sem_open failed");
// 			return ;
// 		}
// 		i++;
// 	}
// }

void	init_philos(t_philo *philo, long long pnumber)
{
	int i;
	t_table *table;

	i = 0;
	table = get_table();
	while (i < pnumber)
	{
		philo[i].id = i + 1;
		philo[i].meals_eaten = 0;
		philo[i].last_meal = getcurrtime();
		philo[i].left_fork = &table->forks[i];
		philo[i].right_fork = &table->forks[(i + 1) % pnumber];
		philo[i].is_dead = false;
		philo[i].last_meal = sem_open("last_meal_sem", O_CREAT, 0644, 1);
		if (philo[i].last_meal == SEM_FAILED)
			pop_error("Error: sem_open failed\n");
		philo[i].meals_sem = sem_open("meal_mutex_sem", O_CREAT, 0644, 1);
		if (philo[i].meals_sem == SEM_FAILED)
			pop_error("Error: sem_open failed\n");
		i++;
	}
}


void	init_table(t_data *data, int ac, char **av)
{
	int		i;
	t_table	*table;
	// sem_t 	sem;
	
	i = 0;
	table = get_table();
	init_data(data, ac, av);
	table->data = data;
	table->dead = false;
	table->start_time = getcurrtime();
	table->log_sem = sem_open("log_sem", O_CREAT, 0644, 1);
	if (table->log_sem == SEM_FAILED)
		pop_error("Error: sem_open failed\n");
	table->dead_sem = sem_open("dead_sem", O_CREAT, 0644, 1);
	if (table->dead_sem == SEM_FAILED)
		pop_error("Error: sem_open failed\n");
	// init_forks(table->forks, data->nb_of_philos);
	table->forks = sem_open("forks", O_CREAT, 0644, data->nb_of_philos);
	if (table->forks == SEM_FAILED)
		pop_error("Error : sem_open failed\n");
	init_philos(table->philos, data->nb_of_philos);
}

int main(int ac, char **av)
{
	t_data	data;

	if (ac != 5 && ac != 6)
		pop_error(USAGE);
	init_table(&data, ac, av);
	// start_simulation();
	return (0);
}