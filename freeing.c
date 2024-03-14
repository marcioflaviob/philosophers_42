/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrandao <mbrandao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 20:21:47 by mbrandao          #+#    #+#             */
/*   Updated: 2024/03/14 21:17:20 by mbrandao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	free_philos(t_philo *philos)
{
	int		i;

	i = 0;
	while (i < philos[0].table->philo_num)
		free(philos[i++].thread);
	free(philos);
}

void	free_forks(t_table *table)
{
	pthread_mutex_t	**forks;
	int				i;

	i = 0;
	forks = table->forks;
	while (i < table->philo_num)
	{
		pthread_mutex_destroy(forks[i]);
		free(forks[i++]);
	}
	free(forks);
}
