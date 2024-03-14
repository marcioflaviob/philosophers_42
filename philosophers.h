/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrandao <mbrandao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 23:09:18 by mbrandao          #+#    #+#             */
/*   Updated: 2024/03/14 23:15:29 by mbrandao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <unistd.h>
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
	pthread_t		*thread;
	pthread_mutex_t	**forks;
}				t_table;

typedef struct s_philo
{
	int				id;
	int				l_fork;
	int				r_fork;
	int				is_eating;
	int				eat_counter;
	size_t			last_meal;
	pthread_t		*thread;
	pthread_mutex_t	*eat_lock;
	t_table			*table;
}				t_philo;

void	free_philos(t_philo *philos);
void	free_forks(t_table *table);

#endif