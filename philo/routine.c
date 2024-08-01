#include "philo.h"


void take_forks(t_philo *philo)
{
    if (philo->id % 2 == 0)
	{
        pthread_mutex_lock(philo->right_fork);
        print_status(philo, "has taken a fork");
        pthread_mutex_lock(philo->left_fork);
    }
	else
	{
        pthread_mutex_lock(philo->left_fork);
        print_status(philo, "has taken a fork");
        pthread_mutex_lock(philo->right_fork);
    }
    print_status(philo, "has taken a fork");
}

void put_down_forks(t_philo *philo)
{
    if (philo->id % 2 == 0)
	{
        pthread_mutex_unlock(philo->left_fork);
        pthread_mutex_unlock(philo->right_fork);
    }
	else
	{
        pthread_mutex_unlock(philo->right_fork);
        pthread_mutex_unlock(philo->left_fork);
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
		table->dead = true;
		return (-1);
	}
	take_forks(philo);
	print_status(philo, "is eating");
	philo->meals_eaten++;
	philo->last_meal = getcurrtime();
	if (!table->dead)
		sleep_ms(table->data->time_to_eat);
	put_down_forks(philo);
	if (table->data->meals != -1 && philo->meals_eaten == table->data->meals)
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
    // usleep((philo->id % 1000) * 100);
    while (!table->dead)
    {
        eat_result = eat_state(philo);
        if (eat_result == 1)  // Philosopher has finished all meals
            break;
        else if (eat_result == 0)
        {
            print_status(philo, "is sleeping");
            sleep_ms(table->data->time_to_sleep);
            print_status(philo, "is thinking");
			if (table->data->time_to_eat + table->data->time_to_sleep >= table->data->time_to_die && !table->dead)
				usleep(10);
        }
    }
    return (NULL);
}
void *monitor_routine(void *data)
{
    int     i;
    t_table *table;
    long long current_time;

    i = 0;
    table = get_table();
    usleep(100);
    while (!table->dead && !all_philos_ate_enough(table))
    {
        pthread_mutex_lock(&table->table_mutex);
        current_time = getcurrtime();
        i = 0;
        while (i < table->data->nb_of_philos)
        {
            if (table->data->meals == -1 || table->philos[i].meals_eaten < table->data->meals)
            {
                if ((current_time - table->philos[i].last_meal) > table->data->time_to_die)
                {
                    table->dead = true;
                    printf("%lld  %d died\n", current_time - table->start_time, table->philos[i].id);
                    pthread_mutex_unlock(&table->table_mutex);
                    return (NULL);
                }
            }
            i++;
        }
        pthread_mutex_unlock(&table->table_mutex);
        usleep(1000);
    }
    return (NULL);
}