#include "philo.h"

void	take_forks(t_philo *philo)
{
	if (philo->id % 2)
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
	}
}

void	put_down_forks(t_philo *philo)
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
	if (table->data->nb_of_philos == 1)
	{
		sleep_ms(table->data->time_to_die);
		print_status(philo, "died");
		set_dead_state(table, true);
		return (-1);
	}
	if (!is_someone_dead(table))
	{
		take_forks(philo);
		print_status(philo, "is eating");
		increment_meals_eaten(philo);
		set_last_meal(philo);
		sleep_ms(table->data->time_to_eat);
		put_down_forks(philo);
		if (table->data->meals != -1 && philo->meals_eaten >= table->data->meals)
			return (-1);
	}
	return (0);
}

void *philo_routine(void *pdata)
{
	t_philo *philo;
	t_table *table;

	philo = (t_philo *)pdata;
	table = get_table();
	if (philo->id % 2)
		usleep((table->data->time_to_die / 2));
	while (!is_someone_dead(table))
	{
		if (eat_state(philo) == -1)
			return (NULL);
		if (!is_someone_dead(table))
			print_status(philo, "is thinking");
		if (!is_someone_dead(table))
		{
			print_status(philo, "is sleeping");
    		sleep_ms(get_table()->data->time_to_sleep);
		}
	}
	return (NULL);
}

void	*monitor_routine(void *data)
{
	int		i;
	t_table	*table;

	i = 0;
	table = get_table();
	while (!all_philos_ate_enough(table) && !is_someone_dead(table))
	{
		i = 0;
		while (i < table->data->nb_of_philos)
		{
			pthread_mutex_lock(&table->philos[i].last_meal_mutex);
			if ((getcurrtime() - table->philos[i].last_meal) >= table->data->time_to_die && table->data->nb_of_philos != 1)
			{
				print_status(&table->philos[i], "died");
				set_dead_state(table, true);
				pthread_mutex_unlock(&table->philos[i].last_meal_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(&table->philos[i].last_meal_mutex);
			i++;
		}
	}
	return (NULL);
}
