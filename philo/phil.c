#include "philo.h"

int get_meals_eaten(t_philo *philo)
{
    int meals;

    pthread_mutex_lock(&philo->meals_mutex);
    meals = philo->meals_eaten;
    pthread_mutex_unlock(&philo->meals_mutex);
    return (meals);
}

long long get_last_meal(t_philo *philo)
{
    long long last_meal;

    pthread_mutex_lock(&philo->last_meal_mutex);
    last_meal = philo->last_meal;
    pthread_mutex_unlock(&philo->last_meal_mutex);
    return (last_meal);
}

bool is_someone_dead(t_table *table)
{
    bool result;
    
    pthread_mutex_lock(&table->dead_mutex);
    result = table->dead;
    pthread_mutex_unlock(&table->dead_mutex);
    return (result);
}
void set_dead_state(t_table *table, bool state)
{
    pthread_mutex_lock(&table->dead_mutex);
    table->dead = state;
    pthread_mutex_unlock(&table->dead_mutex);
}

void increment_meals_eaten(t_philo *philo)
{
    pthread_mutex_lock(&philo->meals_mutex);
    philo->meals_eaten++;
    pthread_mutex_unlock(&philo->meals_mutex);
}#include "philo.h"

void	take_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork");
}

void	put_down_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
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
	take_forks(philo);
	print_status(philo, "is eating");
	philo->meals_eaten++;
	philo->last_meal = getcurrtime();
	sleep_ms(table->data->time_to_eat);
	put_down_forks(philo);
	return (0);
}

void *philo_routine(void *pdata)
{
	t_philo *philo;
	t_table *table;

	philo = (t_philo *)pdata;
	table = get_table();
	// if (philo->id % 2 == 0)
	// 	usleep(500);
	while (!table->dead)
	{
		if (eat_state(philo) == 0)
		{
			print_status(philo, "is sleeping");
			sleep_ms(table->data->time_to_sleep);
			print_status(philo, "is thinking");
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
	while (!table->dead && !all_philos_ate_enough(table))
	{
		pthread_mutex_lock(&table->table_mutex);
		i = 0;
		while (i < table->data->nb_of_philos)
		{
			if ((getcurrtime() - table->philos[i].last_meal) > table->data->time_to_die)
			{
				table->dead = true;
				printf("%lld  %d died\n", getcurrtime() - table->start_time, table->philos[i].id);
				return (NULL);
			}
			i++;
		}
		pthread_mutex_unlock(&table->table_mutex);
		usleep(1000);
	}
	return (NULL);
}
