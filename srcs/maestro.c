/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maestro.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrandao <mbrandao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 21:32:53 by mbrandao          #+#    #+#             */
/*   Updated: 2024/03/18 23:14:03 by mbrandao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	print_msg(t_philo *philo, t_table *table, char *msg)
{
	pthread_mutex_lock(table->print_lock);
	if (stop_check(table))
	{
		pthread_mutex_unlock(table->print_lock);
		return (1);
	}
	printf("%zu Philosopher %d %s.\n", (get_current_time() - table->start_time),
		(philo->id + 1), msg);
	pthread_mutex_unlock(table->print_lock);
	return (0);
}

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
			printf("%zu Philosopher %d died.\n",
				(get_current_time() - table->start_time), philos[i].id + 1);
			pthread_mutex_unlock(philos[i++].eat_lock);
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
	}
	if (counter == philos[0].table->philo_num)
	{
		printf("Everybody ate %d time(s).\n\n", philos[0].table->eat_num);
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
		pthread_mutex_lock(philos[0].table->print_lock);
		if (all_ate(philos) || is_dead(philos))
		{
			pthread_mutex_lock(philos[0].table->stop_lock);
			philos[0].table->stop = 1;
			pthread_mutex_unlock(philos[0].table->stop_lock);
			pthread_mutex_unlock(philos[0].table->print_lock);
			break ;
		}
		pthread_mutex_unlock(philos[0].table->print_lock);
	}
	return (ptr);
}
