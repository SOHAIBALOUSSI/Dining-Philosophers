/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_fetch.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sait-alo <sait-alo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 18:19:42 by sait-alo          #+#    #+#             */
/*   Updated: 2024/08/16 18:27:26 by sait-alo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	is_someone_dead(t_table *table)
{
	bool	result;

	pthread_mutex_lock(&table->dead_mtx);
	result = table->dead;
	pthread_mutex_unlock(&table->dead_mtx);
	return (result);
}

void	set_dead_state(bool state)
{
	t_table	*table;

	table = get_table();
	pthread_mutex_lock(&table->dead_mtx);
	table->dead = state;
	pthread_mutex_unlock(&table->dead_mtx);
}
