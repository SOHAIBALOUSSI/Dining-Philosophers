#include "philo.h"



/*
if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
	}
*/

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


	// if (philo->id % 2 == 0)
	// {
	// 	pthread_mutex_lock(philo->left_fork);
	// 	print_status(philo, "has taken a fork");
	// 	pthread_mutex_lock(philo->right_fork);
	// }
	// else
	// {
	// 	pthread_mutex_lock(philo->right_fork);
	// 	print_status(philo, "has taken a fork");
	// 	pthread_mutex_lock(philo->left_fork);
	// }


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

void set_last_meal(t_philo *philo)
{
	pthread_mutex_lock(&philo->last_meal_mutex);
	philo->last_meal = getcurrtime();
	pthread_mutex_unlock(&philo->last_meal_mutex);
}

static int eat_state(t_philo *philo)
{
	t_table *table;
	
	table = get_table();
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
	int eat_result;

	philo = (t_philo *)pdata;
	table = get_table();
	if (philo->id % 2)
		usleep(100);
	while (!table->dead)
	{
        // printf("1");
        if (table->data->nb_of_philos == 1)
            break;
		if (!eat_state(philo))
		{
			print_status(philo, "is thinking");
			print_status(philo, "is sleeping");
			sleep_ms(table->data->time_to_sleep);
		}
	}
	return (NULL);
}
void *monitor_routine(void *data)
{
	int     i;
	t_table *table;
	long long ct;

	(void)data;
	table = get_table();
	usleep(100);
	while (!table->dead && !all_philos_ate_enough(table))
	{
		i = 0;
		while (i < table->data->nb_of_philos)
		{
			if (table->data->meals == -1 || table->philos[i].meals_eaten < table->data->meals)
			{
				ct = getcurrtime();
				if ((ct - table->philos[i].last_meal) >= table->data->time_to_die)
				{
					table->dead = true;
					pthread_mutex_lock(&table->log_mutex);
					printf(RED"%lld  %d died\n"RESET, ct - table->start_time, table->philos[i].id);
					pthread_mutex_unlock(&table->log_mutex);
					return (NULL);
				}
			}
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}
