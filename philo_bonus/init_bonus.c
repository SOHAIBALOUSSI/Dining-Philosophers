/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sait-alo <sait-alo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 18:19:42 by sait-alo          #+#    #+#             */
/*   Updated: 2024/08/16 19:06:11 by sait-alo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

t_time	get_lastmeal(t_philo *philo)
{
	t_table	*table;
	t_time	lastmeal;

	table = get_table();
	sem_wait(table->last_meal_sem);
	lastmeal = philo->last_meal;
	sem_post(table->last_meal_sem);
	return (lastmeal);
}

void	clean_table(void)
{
	int		i;
	t_table	*table;

	i = 0;
	table = get_table();
	free(table->pids);
	sem_close(table->forks);
	sem_close(table->log_sem);
	sem_close(table->dead_sem);
	sem_close(table->last_meal_sem);
	sem_close(table->full_sem);
	sem_unlink("/forks");
	sem_unlink("/log");
	sem_unlink("/dead");
	sem_unlink("/last_meal");
	sem_unlink("/full");
}

static int	ft_atoi(char *str)
{
	int		i;
	long	result;

	i = 0;
	result = 0;
	while (str[i] == ' ' || str[i] == '\n' || str[i] == '\t')
		i++;
	if (str[i] == '+' && str[i + 1] != '-')
		i++;
	if (str[i] == '-')
		pop_error("Error: arguments cannot be set to negative values\n");
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			pop_error("Error: only numbers are allowed\n");
		result = result * 10;
		if (result > 2147483647)
			pop_error("Error: number is too big\n");
		result += str[i] - '0';
		i++;
	}
	return (result);
}

void	init_data(t_data *data, int ac, char **av)
{
	data->nb_of_philos = ft_atoi(av[1]);
	data->ttd = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->meals = -1;
	if (ac == 6)
		data->meals = ft_atoi(av[5]);
	if (!data->meals || !data->nb_of_philos || !data->ttd \
		|| !data->time_to_eat || !data->time_to_sleep)
		pop_error("Error: this arg(s) cannot be set to 0\n");
	if (data->nb_of_philos > MAX_PHILOS)
		pop_error("Error: too many philosophers\n");
	if (data->ttd < MIN_TIME \
		|| data->time_to_eat < MIN_TIME \
		|| data->time_to_sleep < MIN_TIME)
		pop_error("Error: time is too short\n");
}
