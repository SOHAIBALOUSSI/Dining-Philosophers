#include "philo_bonus.h"
#include "string.h"

t_table	*get_table(void)
{
	static t_table table;
	return (&table);
}

void sleep_ms(long long ms)
{
	long long start;

	start = getcurrtime();
	while (getcurrtime() - start < ms)
		usleep(50);
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

void	init_philos(t_philo *philo, long long pnumber)
{
	int	i;

	i = 0;
	while (i < pnumber)
	{
		philo[i].id = i + 1;
		philo[i].meals_eaten = 0;
		philo[i].last_meal = getcurrtime();
		philo[i].is_dead = false;
		philo[i].is_full = false;
		i++;
	}
}


void	init_table(t_data *data, int ac, char **av)
{
	t_table *table;

	table = get_table();
	init_data(data, ac, av);
	table->data = data;
	table->start_time = getcurrtime();
	sem_unlink("/forks");
	sem_unlink("/log");
	sem_unlink("/dead");
	sem_unlink("/last_meal");
	sem_unlink("/full");
	table->full_sem = sem_open("/full", O_CREAT, 0644, 0);
	table->last_meal_sem = sem_open("/last_meal", O_CREAT, 0644, 1);
	table->forks = sem_open("/forks", O_CREAT, 0644, data->nb_of_philos);
	table->log_sem = sem_open("/log", O_CREAT, 0644, 1);
	table->dead_sem = sem_open("/dead", O_CREAT, 0644, 0);
	table->pids = malloc(sizeof(pid_t) * data->nb_of_philos);
	table->finish_eat = 0;
	if (!table->pids)
		pop_error("malloc failed!");
	if (!table->forks || !table->log_sem || !table->dead_sem || !table->last_meal_sem || !table->full_sem)
		pop_error("Error : sem_open failed\n");
	init_philos(table->philos, data->nb_of_philos);
}

void	print_status(t_philo *philo, char *status)
{
	t_table *table;

	table = get_table();
	sem_wait(table->log_sem);
	if (!table->dead || (table->data->meals != -1 && philo->meals_eaten >= table->data->meals))
		printf("%lld  %d %s\n", getcurrtime() - table->start_time, philo->id, status);
	sem_post(table->log_sem);
}

void	*monitor_routine(void *data)
{
	t_philo	*philo;
	t_table	*table;
	t_time	current_time;

	philo = (t_philo *)data;
	table = get_table();
	while (!table->dead)
	{
		current_time = getcurrtime();
		sem_wait(table->last_meal_sem);
		if ((current_time - philo->last_meal) >= table->data->time_to_die)
		{
			table->dead = true;
			sem_wait(table->log_sem);
			printf(RED"%lld  %d %s\n"RESET, getcurrtime() - table->start_time, philo->id, "died");
			sem_close(table->log_sem);
			sem_post(table->dead_sem);
			sem_post(table->last_meal_sem);
			return (NULL);
		}
		sem_post(table->last_meal_sem);
		usleep(1000);
	}
	return (NULL);
}

void philosopher_actions(t_philo *philo, t_table *table)
{
	print_status(philo, "is thinking");
	sem_wait(table->forks);
	print_status(philo, "has taken a fork");
	sem_wait(table->forks);
	print_status(philo, "has taken a fork");
	print_status(philo, "is eating");
    sem_wait(table->last_meal_sem);
    philo->last_meal = getcurrtime();
    philo->meals_eaten++;
    if (table->data->meals != -1 && philo->meals_eaten >= table->data->meals)
		sem_post(table->full_sem);
    sem_post(table->last_meal_sem);
	sleep_ms(table->data->time_to_eat);
	sem_post(table->forks);
	sem_post(table->forks);
	print_status(philo, "is sleeping");
	sleep_ms(table->data->time_to_sleep);
}

void	*run_philo(void *data)
{
	t_philo	*philo;
	t_table	*table;

	philo = (t_philo *)data;
	table = get_table();
	if (pthread_create(&philo->monitor, NULL, monitor_routine, philo))
		pop_error("pthread_create failed!\n");
	if (pthread_detach(philo->monitor))
		pop_error("pthread_join failed\n");
	while (!table->dead && philo->is_full == false)
		philosopher_actions(philo, table);
	return (NULL);
}
void	*check_full(void *data)
{
	t_table	*table;
	int		i;

	table = get_table();
	i = 0;
	while (i < table->data->nb_of_philos)
	{
		sem_wait(table->full_sem);
		i++;
	}
	sem_post(table->dead_sem);
	table->dead = true;
}

void	start_simulation(void)
{
	int		i;
	t_table	*table;

	i = 0;
	table = get_table();
	if (pthread_create(&table->boss, NULL, check_full, NULL))
		pop_error("pthread_create failed\n");
	if (pthread_detach(table->boss))
		pop_error("pthread_join failed\n");
	while (i < table->data->nb_of_philos)
	{
		table->pids[i] = fork();
		if (table->pids[i] == -1)
			pop_error("fork failed\n");
		if (table->pids[i] == 0)
		{
			run_philo(&table->philos[i]);
			exit(0);
		}
		i++;
	}
}

void	end_simulation(void)
{
	int		i;
	t_table	*table;

	i = 0;
	table = get_table();
	sem_wait(table->dead_sem);
	table->dead = true;
	while (i < table->data->nb_of_philos)
	{
		if (kill(table->pids[i], SIGKILL))
			pop_error("kill failed\n");
		i++;
	}
}

int main(int ac, char **av)
{
	t_data	data;

	if (ac != 5 && ac != 6)
		pop_error(USAGE);
	init_table(&data, ac, av);
	start_simulation();
	end_simulation();
	return (0);
}