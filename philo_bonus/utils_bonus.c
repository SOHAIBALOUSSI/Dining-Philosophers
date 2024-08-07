#include "philo_bonus.h"

void pop_error(char *msg)
{
	size_t size;

	size = 0;
	while (msg[size])
		size++;
	write(2, msg, size);
	exit(EXIT_FAILURE);
}

 
t_time getcurrtime(void)
{
	struct timeval tv;

	if (gettimeofday(&tv, NULL))
		return (pop_error("Error: gettimeofday failed\n"), -1);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

bool    all_philos_ate_enough(t_table *table)
{
	int		i;

	if (table->data->meals == -1)
		return (false);
	i = 0;
	while (i < table->data->nb_of_philos)
	{
		if (table->philos[i].meals_eaten < table->data->meals)
			return (false);
		i++;
	}
	return (true);
}