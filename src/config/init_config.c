/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_config.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hganet <hganet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:24:33 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/13 11:26:41 by hganet           ###   ########.fr       */
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
		philos[i].id = i + 1;
		philos[i].meals_eaten = 0;
		philos[i].last_meal = get_timestamp_ms();
		philos[i].left_fork = &forks[i];
		philos[i].right_fork = &forks[(i + 1) % config->nb_philo]; // Handle wrap around : (4 + 1) % 5 = 0
		philos[i].config = config;
		i++;
	}
	return (0);
}

int init_simulation(t_config *config, t_philo **philos, pthread_mutex_t **forks)
{
	*philos = malloc(sizeof(t_philo) * config->nb_philo);
	*forks = malloc(sizeof(pthread_mutex_t) * config->nb_philo);
	if (!*philos || !*forks)
		return (0);
	config->start_time = get_timestamp_ms();
	if (init_forks(*forks, config->nb_philo))
		return (0);
	if (init_philos(*philos, *forks, config))
		return (0);
	if (pthread_mutex_init(&config->print_mutex, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&config->death_mutex, NULL) != 0)
		return (0);
	config->stop_simulation = 0;
	return (1);
}
