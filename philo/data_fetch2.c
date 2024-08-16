/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_fetch2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sait-alo <sait-alo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 18:19:42 by sait-alo          #+#    #+#             */
/*   Updated: 2024/08/16 18:53:00 by sait-alo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	is_full(t_philo *philo)
{
	t_table	*table;
	bool	is_full;

	table = get_table();
	pthread_mutex_lock(&table->table_mutex);
	is_full = philo->is_full;
	pthread_mutex_unlock(&table->table_mutex);
	return (is_full);
}

int	get_meals(t_philo *philo)
{
	int	meals;

	pthread_mutex_lock(&philo->meals_mutex);
	meals = philo->meals_eaten;
	pthread_mutex_unlock(&philo->meals_mutex);
	return (meals);
}

t_time	lastm(t_philo *philo)
{
	t_time	last_meals;

	pthread_mutex_lock(&philo->last_meal_mutex);
	last_meals = philo->last_meal;
	pthread_mutex_unlock(&philo->last_meal_mutex);
	return (last_meals);
}

void	set_full(t_philo *philo)
{
	t_table	*table;

	table = get_table();
	pthread_mutex_lock(&table->table_mutex);
	philo->is_full = true;
	pthread_mutex_unlock(&table->table_mutex);
}
