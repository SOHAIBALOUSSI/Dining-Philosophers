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
	if (ms - 10 > 0)
		usleep(ms - 10);
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
	int i;
	t_table *table;

	i = 0;
	table = get_table();
	while (i < pnumber)
	{
		philo[i].id = i + 1;
		philo[i].meals_eaten = 0;
		philo[i].last_meal = getcurrtime();
		philo[i].is_dead = false;
		i++;
	}
}


void	init_table(t_data *data, int ac, char **av)
{
	t_table	*table;

	table = get_table();
	init_data(data, ac, av);
	table->data = data;
	table->dead = false;
	table->start_time = getcurrtime();
	table->forks = sem_open("/forks", O_CREAT, 0644, data->nb_of_philos);
	table->log_sem = sem_open("/log_sem", O_CREAT, 0644, 1);
	table->dead_sem = sem_open("/dead_sem", O_CREAT, 0644, 1);
	table->pids = malloc(sizeof(int) * data->nb_of_philos);
	if (!table->pids)
		pop_error("malloc failed!");
	if (!table->forks || !table->log_sem || !table->dead_sem)
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

void *monitor_routine(void *data)
{
    t_philo *philo = (t_philo *)data;
    t_table *table = get_table();
    t_time current_time;

    while (!table->dead)
	{
        current_time = getcurrtime();
        if ((current_time - philo->last_meal) > table->data->time_to_die)
		{
            table->dead = true;
            print_status(philo, "died");
            sem_post(table->dead_sem);
			return (NULL);
        }
        usleep(1000);
    }
    return NULL;
}
int	eat_state(t_philo *philo)
{
	t_table	*table;
	table = get_table();
	
	sem_wait(table->forks);
	sem_wait(table->forks);
	print_status(philo, "has taken a fork");
	print_status(philo, "has taken a fork");
	print_status(philo, "is eating");
	philo->last_meal = getcurrtime();
	if (!table->dead)
		sleep_ms(table->data->time_to_eat);
	philo->meals_eaten++;
	sem_post(table->forks);
	sem_post(table->forks);
	// if (table->data->meals != -1 && philo->meals_eaten >= table->data->meals)
	// 	return (1);
	return (0);
}


void	*run_philo(t_philo *pdata)
{
	t_table *table;
	t_philo	*philo;

	philo = (t_philo *)pdata;
	table = get_table();

	if (pthread_create(&pdata->monitor, NULL, monitor_routine, pdata))
		pop_error("pthread_create failed!\n");
	if (pthread_detach(pdata->monitor))
		pop_error("pthread_detach failed\n");
	while (!table->dead)
	{
		if (eat_state(philo))
			break ;
		print_status(philo, "is sleeping");
		sleep_ms(table->data->time_to_sleep);
		print_status(philo, "is thinking");
		usleep(100);
	}
	return (NULL);
	
}

void	start_simulation(void)
{
	int		i;
	t_table *table;
	pid_t	pid;

	i = 0;
	table = get_table();
	while (i < table->data->nb_of_philos)
	{
		pid = fork();
		if (pid == 0)
		{
			run_philo(&table->philos[i]);
			
			exit(0);
		}
		table->pids[i] = pid;
		i++;
	}
}

void	pkill(void)
{
	int i = 0;
	t_table *table;

	table = get_table();
	while (i < table->data->nb_of_philos)
	{
		kill(table->pids[i], 9);
		i++;
	}
}
void end_simulation(void)
{
	t_table *table;

	table = get_table();
	sem_wait(table->dead_sem);
	pkill();
}

int main(int ac, char **av)
{
	t_data	data;

	if (ac != 5 && ac != 6)
		pop_error(USAGE);
	init_table(&data, ac, av);
	start_simulation();
	while (1)
	{
		if (get_table()->dead)
			break ;
		if (all_philos_ate_enough(get_table()))
			break ;
	}
	end_simulation();
	return (0);
}