/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sait-alo <sait-alo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 18:19:42 by sait-alo          #+#    #+#             */
/*   Updated: 2024/08/16 19:07:50 by sait-alo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

t_table	*get_table(void)
{
	static t_table	table;

	return (&table);
}

void	pop_error(char *msg)
{
	size_t	size;

	size = 0;
	while (msg[size])
		size++;
	write(2, msg, size);
	exit(EXIT_FAILURE);
}

void	sleep_ms(t_time ms)
{
	t_time	start;

	start = getcurrtime();
	while (getcurrtime() - start < ms)
		usleep(100);
}

t_time	getcurrtime(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (pop_error("Error: gettimeofday failed\n"), -1);
	return (((tv.tv_sec * 1000) + (tv.tv_usec / 1000)));
}

void	print_status(t_philo *philo, char *status)
{
	t_table	*table;

	table = get_table();
	sem_wait(table->log_sem);
	printf("%ld  %d %s\n", \
		getcurrtime() - table->start_time, philo->id, status);
	sem_post(table->log_sem);
}
