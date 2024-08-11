#include "philo.h"

void take_forks(t_philo *philo)
{
	if (philo->id % 2)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
	}
}

void put_down_forks(t_philo *philo)
{
	if (philo->id % 2)
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
}

static int eat_state(t_philo *philo)
{
	t_table *table;
	
	table = get_table();
	if (table->data->nb_of_philos == 1)
	{
		table->dead = true;
		sleep_ms(table->data->time_to_die);
		pthread_mutex_lock(&table->log_mutex);
		printf(RED"%lld  %d died\n"RESET, getcurrtime() - table->start_time, 1);
		pthread_mutex_unlock(&table->log_mutex);
		return (-1);
	}
	take_forks(philo);
	print_status(philo, "is eating");
	philo->last_meal = getcurrtime();
	if (!table->dead)
		sleep_ms(table->data->time_to_eat);
	philo->meals_eaten++;
	put_down_forks(philo);
	if (table->data->meals != -1 && philo->meals_eaten >= table->data->meals)
		return (1);
	return (0);
}

void *philo_routine(void *pdata)
{
	t_philo *philo;
	t_table *table;

	philo = (t_philo *)pdata;
	table = get_table();
	if (philo->id % 2)
		usleep(philo->id * 100);
	while (!table->dead)
	{
		if (!table->dead)
			print_status(philo, "is thinking");
		if (!table->dead && eat_state(philo))
			break;
		if (!table->dead)
		{
			print_status(philo, "is sleeping");
			sleep_ms(table->data->time_to_sleep);
			usleep(100);
		}
	}
	return (NULL);
}
void *monitor_routine(void *data)
{
	t_table *table;
	int i;

	table = get_table();
	// i = 0;
	while (!table->dead && !all_philos_ate_enough(table))
	{
		i = 0;
		while (i < table->data->nb_of_philos)
		{
			if (table->data->meals == -1 || table->data->meals < table->philos[i].meals_eaten)
			{
				if ((getcurrtime() - table->philos[i].last_meal) > table->data->time_to_die)
				{
					table->dead = true;
					pthread_mutex_lock(&table->log_mutex);
					printf(RED"%lld  %d died\n"RESET, getcurrtime() - table->start_time, table->philos[i].id);
					pthread_mutex_unlock(&table->log_mutex);
					return (NULL);
				}
			}
			else
				break;
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}

