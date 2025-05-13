/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_config.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hganet <hganet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:24:33 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/13 12:16:50 by hganet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int init_forks(pthread_mutex_t *forks, int nb_philo)
{
	int i = 0;

	while (i < nb_philo)
	{
		/**
		 * The pthread_mutex_init function initializes a mutex object of type pthread_mutex_t
		 * with optional attributes specified by a pthread_mutexattr_t object.
		 * If the attributes parameter is NULL, default attributes are used;
		 * the function is typically used to prepare a mutex for synchronization in multithreaded programs.
		 */
		if (pthread_mutex_init(&forks[i], NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}

static int init_philos(t_philo *philos, pthread_mutex_t *forks, t_config *config)
{
	int i = 0;

	while (i < config->nb_philo)
	{
		philos[i].id = i + 1; // Philosopher IDs start from 1
		philos[i].meals_eaten = 0; // Initialize meals eaten to 0
		philos[i].last_meal = get_timestamp_ms(); // Set last meal to current time
		philos[i].left_fork = &forks[i]; // Left fork is the current fork
		// Right fork is the next one in the array, with wrap around
		// For example, if there are 5 philosophers, philosopher 4's right fork is philosopher 0's left fork : (4 + 1) % 5 = 0
		philos[i].right_fork = &forks[(i + 1) % config->nb_philo];
		philos[i].config = config; // Set the simulation config pointer for each philosopher so they can access shared data
		i++;
	}
	return (0);
}

int init_simulation(t_config *config, t_philo **philos, pthread_mutex_t **forks)
{ 
	*philos = malloc(sizeof(t_philo) * config->nb_philo); // Allocate memory for philosophers
	if (!*philos)
		return (0);
	*forks = malloc(sizeof(pthread_mutex_t) * config->nb_philo); // Allocate memory for forks mutexes
	if (!*forks)
	{
		free(*philos); // Free philosophers if forks allocation fails
		return (0);
	}
	config->start_time = get_timestamp_ms(); // Get the current timestamp for simulation start
	if (init_forks(*forks, config->nb_philo)) // Initialize forks mutexes to prepare for synchronization
		return (0);
	if (init_philos(*philos, *forks, config)) // Initialize philosophers with their IDs, forks, and config
		return (0);
	if (pthread_mutex_init(&config->print_mutex, NULL) != 0) // Initialize print mutex for synchronized output
		return (0);
	if (pthread_mutex_init(&config->death_mutex, NULL) != 0) // Initialize death mutex for synchronized access to death status
		return (0);
	config->stop_simulation = 0; // Initialize stop simulation flag to 0
	return (1);
}
