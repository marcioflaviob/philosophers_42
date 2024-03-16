/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrandao <mbrandao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 23:09:18 by mbrandao          #+#    #+#             */
/*   Updated: 2024/03/16 22:14:43 by mbrandao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <unistd.h>
# include <limits.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_table
{
	int				philo_num;
	int				time_starve;
	int				time_eat;
	int				time_sleep;
	int				eat_num;
	int				stop;
	long int		start_time;
	pthread_t		*thread;
	pthread_mutex_t	*stop_lock;
	pthread_mutex_t	**forks;
}				t_table;

typedef struct s_philo
{
	int				id;
	int				l_fork;
	int				r_fork;
	int				is_eating;
	int				eat_counter;
	long int		last_meal;
	pthread_t		*thread;
	pthread_mutex_t	*eat_lock;
	t_table			*table;
}				t_philo;

void		free_philos(t_philo *philos);

long int	get_current_time(void);
void		ft_usleep(long int time_in_ms);
int			stop_check(t_table *table);
long		ft_atoi(const char *str);

void		*maestro(void *ptr);

void		eat(t_philo *philo, t_table *table);
void		think(t_philo *philo);
void		sleep_state(t_philo *philo, t_table *table);
void		*routine(void *void_philo);

#endif