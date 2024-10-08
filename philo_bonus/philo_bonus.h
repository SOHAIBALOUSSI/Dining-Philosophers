/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sait-alo <sait-alo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 18:19:42 by sait-alo          #+#    #+#             */
/*   Updated: 2024/08/16 19:06:27 by sait-alo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>
# include <stdbool.h>
# include <sys/time.h>
# include <semaphore.h>
# include <fcntl.h>
# include <sys/types.h>
# include <signal.h>
# include <sys/wait.h>

# define RED "\033[0;31m"
# define RESET  "\033[0m"

# define MAX_PHILOS 337

# define MIN_TIME 42
# define USAGE "[nb_of_philos][ttd][time_to_eat][time_to_sleep]*[meals]\n"

typedef long	t_time;

typedef struct s_data
{
	t_time	ttd;
	t_time	time_to_eat;
	t_time	time_to_sleep;
	int		nb_of_philos;
	int		meals;
}				t_data;

typedef struct s_philo
{
	pthread_t	monitor;
	t_time		last_meal;
	int			id;
	int			meals_eaten;
}				t_philo;

typedef struct s_table
{
	t_data			*data;
	t_philo			philos[MAX_PHILOS];
	pthread_t		boss;
	pid_t			*pids;
	sem_t			*forks;
	sem_t			*log_sem;
	sem_t			*dead_sem;
	sem_t			*last_meal_sem;
	sem_t			*full_sem;
	t_time			start_time;
}				t_table;

t_table	*get_table(void);

void	init_data(t_data *data, int ac, char **av);
void	*spawn_philo(void *data);

/*		Utils		*/
void	print_status(t_philo *philo, char *status);
void	pop_error(char *msg);
t_time	getcurrtime(void);
void	sleep_ms(t_time ms);
void	clean_table(void);
t_time	get_lastmeal(t_philo *philo);

#endif /* PHILO_BONUS */