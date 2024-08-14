#include "philo_bonus.h"
#include "string.h"

t_table	*get_table(void)
{
	static t_table table;
	return (&table);
}
__attribute__((destructor)) void	destroyer(void)
{
	int i = 0;
	t_table *table;

	table = get_table();
	free(table->pids);
	sem_close(table->forks);
	sem_close(table->log_sem);
	sem_close(table->dead_sem);
	sem_close(table->last_meal_sem);
	sem_close(table->full_sem);
	sem_close(table->died);
	sem_unlink("/forks");
	sem_unlink("/log");
	sem_unlink("/dead");
	sem_unlink("/last_meal");
	sem_unlink("/full");
	sem_unlink("/died");
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
	sem_unlink("/died");
	table->died = sem_open("/died", O_CREAT, 0644, 1);
	table->full_sem = sem_open("/full", O_CREAT, 0644, data->nb_of_philos);
	table->last_meal_sem = sem_open("/last_meal", O_CREAT, 0644, 1);
	table->forks = sem_open("/forks", O_CREAT, 0644, data->nb_of_philos);
	table->log_sem = sem_open("/log", O_CREAT, 0644, 1);
	table->dead_sem = sem_open("/dead", O_CREAT, 0644, 1);
	table->pids = malloc(sizeof(pid_t) * data->nb_of_philos);
	table->finish_eat = 0;
	if (!table->pids)
		pop_error("malloc failed!\n");
	if (!table->forks || !table->log_sem || !table->dead_sem ||
		!table->last_meal_sem || !table->full_sem || !table->died)
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
	t_time last_meal_time;

	philo = (t_philo *)data;
	table = get_table();
	while (1)
	{
		sem_wait(table->last_meal_sem);
		last_meal_time = philo->last_meal;
		sem_post(table->last_meal_sem);
		if ((getcurrtime() - last_meal_time) > table->data->time_to_die)
		{
			sem_wait(table->died);
			sem_wait(table->log_sem);
			printf(RED"%lld  %d %s\n"RESET, getcurrtime() - table->start_time, philo->id, "died");
			sem_post(table->log_sem);
			sem_wait(table->dead_sem);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}

void	take_forks(t_philo *philo, t_table *table)
{
	sem_wait(table->forks);
	print_status(philo, "has taken a fork");
	sem_wait(table->forks);
	print_status(philo, "has taken a fork");
}

void	eat(t_philo *philo, t_table *table)
{
	print_status(philo, "is eating");
	sem_wait(table->last_meal_sem);
	philo->last_meal = getcurrtime();
	philo->meals_eaten++;
	if (table->data->meals != -1 && philo->meals_eaten >= table->data->meals)
		sem_wait(table->full_sem);	
	sem_post(table->last_meal_sem);
	sleep_ms(table->data->time_to_eat);
}

void	release_forks(t_table *table)
{
	sem_post(table->forks);
	sem_post(table->forks);
}

void	philosopher_actions(t_philo *philo, t_table *table)
{
	print_status(philo, "is thinking");
	usleep(100);
	take_forks(philo, table);
	eat(philo, table);
	release_forks(table);
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
	while (!table->dead)
		philosopher_actions(philo, table);
	return (NULL);
}

void	*check_full(void *data)
{
	int		i;
	t_table	*table;

	i = 0;
	table = get_table();
	while (1)
	{
		if (table->full_sem->__align == 0)
			break;
		usleep(1000);
	}
		// printf("salina f check_full\n");
	// table->dead = true;
	sem_wait(table->dead_sem);
	return (NULL);
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
			printf("makhasoch ykhrj\n");
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
	while (1)
	{
		if (table->dead_sem->__align == 0)
			break;
		usleep(1000);
	}
	printf("salina\n");
	table->dead = true;
	while (i < table->data->nb_of_philos)
	{
		if (kill(table->pids[i], SIGKILL))
			pop_error("kill failed\n");
		i++;
	}
	sem_post(table->died);
	sem_post(table->dead_sem);
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