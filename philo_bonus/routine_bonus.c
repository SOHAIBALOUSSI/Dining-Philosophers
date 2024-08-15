#include "philo_bonus.h"

t_time	get_lastmeal(t_philo *philo)
{
	t_table *table;
	t_time lastmeal;

	table = get_table();
	sem_wait(table->last_meal_sem);
	lastmeal = philo->last_meal;
	sem_post(table->last_meal_sem);
	return (lastmeal);
}

static void	*monitor_routine(void *data)
{
	t_philo	*philo;
	t_table	*table;
	t_time	ct;

	philo = (t_philo *)data;
	table = get_table();
	while (true)
	{
		ct = getcurrtime();
		if ((ct - get_lastmeal(philo)) > table->data->time_to_die)
		{
			sem_wait(table->log_sem);
			printf(RED"%ld  %d %s\n"RESET, ct - table->start_time, philo->id, "died");
			sem_wait(table->dead_sem);
			return (NULL);
		}
	}
	return (NULL);
}

static void	eat(t_philo *philo, t_table *table)
{
	sem_wait(table->forks);
	print_status(philo, "has taken a fork");
	sem_wait(table->forks);
	print_status(philo, "has taken a fork");
	print_status(philo, "is eating");
	sem_wait(table->last_meal_sem);
	philo->last_meal = getcurrtime();
	philo->meals_eaten++;
	if (philo->id == table->data->nb_of_philos \
		&& (table->data->meals != -1 && philo->meals_eaten > table->data->meals))
		sem_wait(table->full_sem);
	sem_post(table->last_meal_sem);
	sleep_ms(table->data->time_to_eat);
	sem_post(table->forks);
	sem_post(table->forks);
}


static void	philo_routine(t_philo *philo, t_table *table)
{
	print_status(philo, "is thinking");
	usleep(500);
	eat(philo, table);
	print_status(philo, "is sleeping");
	sleep_ms(table->data->time_to_sleep);
}

static void    spawn_monitor(t_philo *philo)
{
	if (pthread_create(&philo->monitor, NULL, monitor_routine, philo))
		pop_error("pthread_create failed!\n");
	if (pthread_detach(philo->monitor))
		pop_error("pthread_join failed\n");
}

void	*spawn_philo(void *data)
{
	t_philo	*philo;
	t_table	*table;

	philo = (t_philo *)data;
	table = get_table();
	spawn_monitor(philo);
	while (true)
		philo_routine(philo, table);
	return (NULL);
}
