/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrandao <mbrandao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 23:09:18 by mbrandao          #+#    #+#             */
/*   Updated: 2024/03/03 00:51:29 by mbrandao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>

typedef struct s_philo
{
	int				id;
	int				l_fork;
	int				r_fork;
	pthread_t		thread;
	pthread_mutex_t	*forks;
}				t_philo;

typedef struct s_table
{
	int				philo_num;
	int				time_starve;
	int				time_eat;
	int				time_sleep;
	int				eat_num;
	pthread_mutex_t	*forks;
	t_philo			*philos;
}				t_table;


#endif