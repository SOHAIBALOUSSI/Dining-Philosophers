/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sait-alo <sait-alo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 00:00:00 by sait-alo          #+#    #+#             */
/*   Updated: 2024/08/17 15:57:21 by sait-alo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_table	*get_table(void)
{
	static t_table	table;

	return (&table);
}

static int	start_simulation(void)
{
	int		i;
	t_table	*table;

	i = 0;
	table = get_table();
	table->start_time = getcurrtime();
	while (i < table->data->nb_of_philos)
	{
		if (pthread_create(&table->philos[i].thread, NULL, \
			philo_routine, &table->philos[i]))
			return (error("Error: pthread_create failed %d\n"), -1);
		i++;
	}
	if (!monitor_routine(NULL))
		return (EXIT_SUCCESS);
	return (0);
}

static int	join_threads(void)
{
	int		i;
	t_table	*table;

	i = 0;
	table = get_table();
	while (i < table->data->nb_of_philos)
	{
		if (pthread_join(table->philos[i].thread, NULL))
			return (error("Error: pthread_join failed\n"), -1);
		i++;
	}
	return (0);
}

static int	clean_table(void)
{
	int		i;
	t_table	*table;

	i = 0;
	table = get_table();
	while (i < table->data->nb_of_philos)
	{
		pthread_mutex_destroy(&table->philos[i].last_meal_mutex);
		pthread_mutex_destroy(&table->philos[i].meals_mutex);
		if (pthread_mutex_destroy(&table->forks[i]))
			return (error("Error: pthread_mutex_destroy failed\n"), -1);
		i++;
	}
	if (pthread_mutex_destroy(&table->dead_mtx)
		|| pthread_mutex_destroy(&table->dead_mutex)
		|| pthread_mutex_destroy(&table->log_mutex)
		|| pthread_mutex_destroy(&table->table_mutex)
		|| pthread_mutex_destroy(&table->full_mutex))
		return (error("Error: pthread_mutex_destroy failed\n"), -1);
	return (0);
}

int	main(int ac, char **av)
{
	t_data	data;

	if (ac != 5 && ac != 6)
		return (error("Usage:\n./philo "USE), EXIT_FAILURE);
	if (init_table(&data, ac, av))
		return (EXIT_FAILURE);
	if (start_simulation())
		return (EXIT_FAILURE);
	if (join_threads())
		return (EXIT_FAILURE);
	if (clean_table())
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
