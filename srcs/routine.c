/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrandao <mbrandao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 21:31:07 by mbrandao          #+#    #+#             */
/*   Updated: 2024/03/18 18:04:39 by mbrandao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	eat_helper(t_philo *philo, t_table *table)
{
	pthread_mutex_lock(philo->eat_lock);
	philo->last_meal = get_current_time();
	philo->is_eating = 1;
	pthread_mutex_unlock(philo->eat_lock);
	ft_usleep(table->time_eat);
	pthread_mutex_unlock(table->forks[philo->r_fork]);
	pthread_mutex_unlock(table->forks[philo->l_fork]);
	pthread_mutex_lock(philo->eat_lock);
	philo->eat_counter--;
	philo->is_eating = 0;
	pthread_mutex_unlock(philo->eat_lock);
}

void	eat(t_philo *philo, t_table *table)
{
	pthread_mutex_lock(table->forks[philo->r_fork]);
	if (print_msg(philo, table, "has taken a fork"))
	{
		pthread_mutex_unlock(table->forks[philo->r_fork]);
		return ;
	}
	if (table->philo_num == 1)
	{
		ft_usleep(table->time_starve);
		pthread_mutex_unlock(table->forks[philo->r_fork]);
		return ;
	}
	pthread_mutex_lock(table->forks[philo->l_fork]);
	if (print_msg(philo, table, "has taken a fork")
		|| print_msg(philo, table, "is eating"))
	{
		pthread_mutex_unlock(table->forks[philo->r_fork]);
		pthread_mutex_unlock(table->forks[philo->l_fork]);
		return ;
	}
	eat_helper(philo, table);
}

void	think(t_philo *philo)
{
	if (stop_check(philo->table))
		return ;
	if (print_msg(philo, philo->table, "is thinking"))
		return ;
}

void	sleep_state(t_philo *philo, t_table *table)
{
	if (stop_check(table))
		return ;
	if (print_msg(philo, table, "is sleeping"))
		return ;
	if (stop_check(table))
		return ;
	ft_usleep(table->time_sleep);
	if (stop_check(table))
		return ;
}

void	*routine(void *void_philo)
{
	t_philo	*philo;
	t_table	*table;

	philo = void_philo;
	table = philo->table;
	if (philo->id % 2 == 0)
		ft_usleep(table->time_eat / 10);
	while (1)
	{
		eat(philo, table);
		if (stop_check(table))
			break ;
		sleep_state(philo, table);
		if (stop_check(table))
			break ;
		think(philo);
	}
	return (NULL);
}
