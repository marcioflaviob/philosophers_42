/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maestro.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrandao <mbrandao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 21:32:53 by mbrandao          #+#    #+#             */
/*   Updated: 2024/03/16 22:04:57 by mbrandao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	is_dead(t_philo *philos)
{
	int		i;
	t_table	*table;

	i = 0;
	table = philos[0].table;
	while (i < table->philo_num)
	{
		pthread_mutex_lock(philos[i].eat_lock);
		if ((get_current_time() - philos[i].last_meal >= table->time_starve)
			&& !philos[i].is_eating)
		{
			printf("%zu Philosopher %d died.\n", (get_current_time() - table->start_time), philos[i].id + 1);
			return (1);
		}
		pthread_mutex_unlock(philos[i++].eat_lock);
	}
	return (0);
}

int	all_ate(t_philo *philos)
{
	int	i;
	int	counter;

	if (philos[0].table->eat_num == -1)
		return (0);
	i = 0;
	counter = 0;
	while (i < philos[0].table->philo_num)
	{
		pthread_mutex_lock(philos[i].eat_lock);
		if (philos[i].eat_counter <= 0)
			counter++;
		pthread_mutex_unlock(philos[i++].eat_lock);
		// printf("\nPhilo %d eat counter is %d\n", i, philos[i].eat_counter);
	}
	// printf("\nTotal eaters is %d and num of philos is %d\n", counter, philos[0].table->philo_num);
	if (counter == philos[0].table->philo_num)
	{
		printf("\n\nEverybody ate %d time(s).\n\n", philos[0].table->eat_num);
		return (1);
	}
	return (0);
}

void	*maestro(void *ptr)
{
	int		i;
	t_philo	*philos;

	i = 0;
	philos = (t_philo *)ptr;
	while (1)
	{
		if (all_ate(philos) || is_dead(philos))
		{
			pthread_mutex_lock(philos[0].table->stop_lock);
			philos[0].table->stop = 1;
			pthread_mutex_unlock(philos[0].table->stop_lock);
			break ;
		}
	}
	return (ptr);
}
