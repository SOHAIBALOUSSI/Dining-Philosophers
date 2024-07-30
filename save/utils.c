#include "philo.h"

void error(char *msg)
{
	size_t size;

	size = 0;
	while (msg[size])
		size++;
	write(2, msg, size);
	exit(EXIT_FAILURE);
}

long long getcurrtime(void)
{
	struct timeval tv;

	if (gettimeofday(&tv, NULL))
		return (error("Error: gettimeofday failed\n"), -1);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void sleep_ms(long long ms)
{
	long long start;

	start = getcurrtime();
	while (getcurrtime() - start < ms)
		usleep(50);
}
bool    all_philos_ate_enough(void)
{
	int		i;
	t_table	*table;

	table = get_table();
	if (table->data->meals == -1)
		return (false);
	i = 0;
	while (i < table->data->nb_of_philos)
	{
		if (get_meals_eaten(&table->philos[i]) < table->data->meals)
			return (false);
		i++;
	}
	return (true);
}

void print_status(t_philo *philo, char *status)
{
	t_table *table;
	bool	is_dead;

	table = get_table();
	is_dead = is_someone_dead(table);
	pthread_mutex_lock(&table->log_mutex);
	if (!is_dead)
		printf("%lld %d %s\n", getcurrtime() - table->start_time, philo->id, status);
	pthread_mutex_unlock(&table->log_mutex);
}

