/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrandao <mbrandao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 23:11:34 by mbrandao          #+#    #+#             */
/*   Updated: 2024/03/16 01:12:30 by mbrandao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	stop_check(t_table *table)
{
	pthread_mutex_lock(table->stop_lock);
	if (table->stop)
	{
		pthread_mutex_unlock(table->stop_lock);
		return (1);
	}
	pthread_mutex_unlock(table->stop_lock);
	return (0);
}

long int	get_current_time(void)
{
	long int			time;
	struct timeval		current_time;

	time = 0;
	if (gettimeofday(&current_time, NULL) == -1)
		; // ERROR
	time = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	return (time);
}

void	ft_usleep(long int time_in_ms)
{
	long int	start_time;

	start_time = 0;
	start_time = get_current_time();
	while ((get_current_time() - start_time) < time_in_ms)
		usleep(time_in_ms / 10);
}

void	eat(t_philo *philo, t_table *table)
{
	pthread_mutex_lock(table->forks[philo->r_fork]);
	if (stop_check(table))
		return ;
	printf("%zu Philosopher %d has taken a fork.\n", (get_current_time() - table->start_time), philo->id + 1);
	if (table->philo_num == 1)
	{
		ft_usleep(table->time_starve);
		pthread_mutex_unlock(table->forks[philo->r_fork]);
		return;
	}
	pthread_mutex_lock(table->forks[philo->l_fork]);
	if (stop_check(table))
		return ;
	printf("%zu Philosopher %d has taken a fork.\n", (get_current_time() - table->start_time), philo->id + 1);
	printf("%zu Philosopher %d is eating.\n", (get_current_time() - table->start_time), philo->id + 1);
	pthread_mutex_lock(philo->eat_lock);
	philo->last_meal = get_current_time();
	philo->is_eating = 1;
	pthread_mutex_unlock(philo->eat_lock);
	ft_usleep(table->time_eat);
	pthread_mutex_unlock(table->forks[philo->r_fork]);
	pthread_mutex_unlock(table->forks[philo->l_fork]);

    // if (philo->id % 2 == 0)
    // {
    //     pthread_mutex_unlock(table->forks[philo->r_fork]);
    //     pthread_mutex_unlock(table->forks[philo->l_fork]);
    // }
    // else
    // {
    //     pthread_mutex_unlock(table->forks[philo->l_fork]);
    //     pthread_mutex_unlock(table->forks[philo->r_fork]);
    // }

	pthread_mutex_lock(philo->eat_lock);
	philo->eat_counter--;
	philo->is_eating = 0;
	pthread_mutex_unlock(philo->eat_lock);
}

void	think(t_philo *philo)
{
	if (stop_check(philo->table))
		return ;
	printf("%zu Philosopher %d is thinking.\n", (get_current_time() - philo->table->start_time), philo->id + 1);
}

void	sleep_state(t_philo *philo, t_table *table)
{
	if (stop_check(table))
		return ;
	printf("%zu Philosopher %d is sleeping.\n", (get_current_time() - table->start_time), philo->id + 1);
	if (stop_check(table))
			return ;
	ft_usleep(table->time_sleep);
	if (stop_check(table))
			return ;
}


// void	*routine()
// {
// 	void *result = NULL;
// 	printf("Philo initiated.\n");
// 	sleep(3);
// 	printf("Finished philo.\n");
// 	return result;
// }


void	*routine(void *void_philo)
{
	t_philo	*philo;
	t_table	*table;
	int		temp_fork;

	philo = void_philo;
	table = philo->table;
	if (philo->id % 2 == 0)
	{
		usleep(50);
		temp_fork = philo->l_fork;
		philo->l_fork = philo->r_fork;
		philo->r_fork = temp_fork;
	}
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
	//WRITE THE LOGIC TO CHECK IF IT'S DEAD
	// THE TIME NOW MINUS THE TIME OF LAST MEAL HAS TO BE BIGGER THAN THE TIME TO STARVE IN ORDER TO BE DEAD, AND CHECK IF ITS NOT EATING AT THE MOMENT

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

void	*program(void *ptr)
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
	// printf("\n\n\n SOMEONE DIED OR ALL ATE \n\n\n");
	return (ptr);
}

t_philo	*create_philos(t_table *table)
{
	int				i;
	t_philo			*philos;

	i = 0;
	philos = (t_philo *) malloc (table->philo_num * sizeof(t_philo));
	if (!philos)
		return (NULL);
	table->stop = 0;
	table->start_time = get_current_time();
	table->stop_lock = (pthread_mutex_t *) malloc (sizeof(pthread_mutex_t));
	pthread_mutex_init(table->stop_lock, NULL);
	while (i < table->philo_num)
	{
		philos[i].id = i;
		philos[i].r_fork = i;
		philos[i].l_fork = i - 1;
		philos[i].eat_counter = table->eat_num;
		philos[i].table = table;
		philos[i].last_meal = 0;
		philos[i].is_eating = 0;
		if (table->philo_num == 1)
			philos[i].l_fork = -1;
		if (i == 0 && table->philo_num > 1)
			philos[i].l_fork = (table->philo_num - 1);
		philos[i].thread = (pthread_t *) malloc (sizeof(pthread_t));
		table->forks[i] = (pthread_mutex_t *) malloc (sizeof(pthread_mutex_t));
		philos[i].eat_lock = (pthread_mutex_t *) malloc (sizeof(pthread_mutex_t));
		pthread_mutex_init(philos[i].eat_lock, NULL);
		pthread_mutex_lock(philos[i].eat_lock);
		philos[i].last_meal = get_current_time();
		pthread_mutex_unlock(philos[i].eat_lock);
		pthread_mutex_init(table->forks[i++], NULL);
	}
	i = 0;
	while (i < table->philo_num)
	{
		pthread_create(philos[i].thread, NULL, &routine, &philos[i]);
		i++;
	}
	return (philos);
}

void	program_start(t_philo *philos)
{
	t_table	*table;

	table = philos[0].table;
	table->thread = (pthread_t *) malloc (sizeof(pthread_t));
	// if (!table->thread)
		//ERROR
	pthread_create(table->thread, NULL, &program, philos);
}


int	main()
{
	t_table			table;
	pthread_mutex_t	**forks;
	t_philo			*philos;

	table.philo_num = 5;
	forks = (pthread_mutex_t **)malloc(table.philo_num
			* sizeof(pthread_mutex_t *));
	table.time_starve = 800;
	table.time_eat = 200;
	table.time_sleep = 200;
	table.eat_num = 10;
	table.forks = forks;
	if (table.philo_num > 200)
		return (0);
	philos = create_philos(&table);
	program_start(philos);
	pthread_join(*(table.thread), NULL);
	free(table.thread);
	free_philos(philos);
	return (0);
}

