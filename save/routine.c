#include "philo.h"

static void	sleep_state(t_philo *philo)
{
    print_status(philo, "is sleeping");
    sleep_ms(get_table()->data->time_to_sleep);
}

static void	think_state(t_philo *philo)
{
	print_status(philo, "is thinking");
}

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
static void eat_state(t_philo *philo)
{
    t_table *table;
	
	table = get_table();
    if (table->data->nb_of_philos == 1)
    {
		set_last_meal(philo);
        sleep_ms(table->data->time_to_die);
        return;
    }
    take_forks(philo);
    print_status(philo, "is eating");
	increment_meals_eaten(philo);
	set_last_meal(philo);
    sleep_ms(table->data->time_to_eat);
	put_down_forks(philo);
}

void *philo_routine(void *pdata)
{
	t_philo *philo;
	t_table *table;

	philo = (t_philo *)pdata;
	table = get_table();
	if (philo->id % 2 == 0)
		sleep_ms(1000);
	while (!is_someone_dead(table))
	{
		eat_state(philo);
		if (table->data->meals != -1 && get_meals_eaten(philo) == table->data->meals)
			return (NULL);
		if (!is_someone_dead(table))
			think_state(philo);
		if (!is_someone_dead(table))
			sleep_state(philo);
	}
	return (NULL);
}
int get_meals_eaten(t_philo *philo)
{
    int result;
    pthread_mutex_lock(&philo->meals_mutex);
    result = philo->meals_eaten;
    pthread_mutex_unlock(&philo->meals_mutex);
    return result;
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
}

void	*monitor_routine(void *data)
{
	int		i;
	t_table	*table;

	i = 0;
	table = get_table();
	while (!is_someone_dead(table) && !all_philos_ate_enough())
	{
		i = 0;
		while (i < table->data->nb_of_philos)
		{
			pthread_mutex_lock(&table->philos[i].last_meal_mutex);
			if (table->philos[i].last_meal != 0 && (getcurrtime() - table->philos[i].last_meal) >= table->data->time_to_die)
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
