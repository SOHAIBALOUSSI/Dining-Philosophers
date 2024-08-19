/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   program_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sait-alo <sait-alo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 18:19:42 by sait-alo          #+#    #+#             */
/*   Updated: 2024/08/17 13:41:18 by sait-alo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	error(char *msg)
{
	size_t	size;

	size = 0;
	while (msg[size])
		size++;
	write(2, msg, size);
}

t_time	getcurrtime(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	sleep_ms(t_time ms)
{
	t_time	start;
	t_table	*table;

	table = get_table();
	start = getcurrtime();
	if (table->data->nb_of_philos == 1)
	{
		while (getcurrtime() - start < ms)
			usleep(100);
	}
	else
	{
		while (!is_someone_dead(table) && getcurrtime() - start < ms)
			usleep(100);
	}
}

bool	all_philos_full(t_table *table)
{
	int	i;

	if (table->data->meals == -1)
		return (false);
	i = 0;
	while (i < table->data->nb_of_philos)
	{
		if (is_full(&table->philos[i]) == false)
			return (false);
		i++;
	}
	return (true);
}

void	print_status(t_philo *philo, char *status)
{
	t_table	*table;

	table = get_table();
	pthread_mutex_lock(&table->log_mutex);
	if (!is_someone_dead(table))
	{
		printf("%ld  %d %s\n", getcurrtime() - table->start_time, \
			philo->id, status);
	}
	pthread_mutex_unlock(&table->log_mutex);
}
