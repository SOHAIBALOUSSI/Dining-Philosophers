/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sait-alo <sait-alo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 18:19:42 by sait-alo          #+#    #+#             */
/*   Updated: 2024/08/17 16:16:54 by sait-alo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	take_forks(t_philo *philo)
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
}

static void	put_down_forks(t_philo *philo)
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

static int	eat_state(t_philo *philo)
{
	t_table	*table;

	table = get_table();
	if (table->data->nb_of_philos == 1)
	{
		set_dead_state(true);
		sleep_ms(table->data->time_to_die);
		pthread_mutex_lock(&table->log_mutex);
		printf(RED"%ld  %d died\n"RESET, table->data->time_to_die, 1);
		pthread_mutex_unlock(&table->log_mutex);
		return (-1);
	}
	take_forks(philo);
	print_status(philo, "is eating");
	pthread_mutex_lock(&philo->last_meal_mutex);
	philo->last_meal = getcurrtime();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->last_meal_mutex);
	sleep_ms(table->data->time_to_eat);
	put_down_forks(philo);
	if (table->data->meals != -1 && get_meals(philo) >= table->data->meals)
		set_full(philo);
	return (0);
}

void	*philo_routine(void *pdata)
{
	t_philo	*philo;
	t_table	*table;

	philo = (t_philo *)pdata;
	table = get_table();
	if (philo->id % 2)
		usleep(500);
	while (!is_someone_dead(table))
	{
		print_status(philo, "is thinking");
		usleep(500);
		if (eat_state(philo))
			break ;
		if (all_full())
			break ;
		print_status(philo, "is sleeping");
		sleep_ms(table->data->time_to_sleep);
	}
	return (NULL);
}

void	*monitor_routine(void *data)
{
	int		i;
	t_table	*table;

	(void)data;
	table = get_table();
	while (!is_someone_dead(table))
	{
		i = 0;
		while (i < table->data->nb_of_philos)
		{
			if (all_philos_full(table))
				return (set_full_state(true), NULL);
			if ((getcurrtime() - get_lastmeal(&table->philos[i]))
				> table->data->time_to_die)
			{
				set_dead_state(true);
				pthread_mutex_lock(&table->log_mutex);
				printf(RED"%ld  %d died\n"RESET, \
					getcurrtime() - table->start_time, table->philos[i].id);
				return (pthread_mutex_unlock(&table->log_mutex), NULL);
			}
			i++;
		}
	}
	return (NULL);
}
