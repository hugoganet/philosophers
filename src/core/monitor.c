/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_function.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 09:07:24 by hugoganet         #+#    #+#             */
/*   Updated: 2025/04/26 16:18:36 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int is_simulation_stopped(t_config *config)
{
	int ret;

	pthread_mutex_lock(&config->death_mutex);
	ret = config->stop_simulation;
	pthread_mutex_unlock(&config->death_mutex);
	//printf("is_simulation_stopped = %i\n", ret);
	return (ret);
}

static void stop_simulation(t_config *config)
{
	pthread_mutex_lock(&config->death_mutex);
	config->stop_simulation = 1;
	pthread_mutex_unlock(&config->death_mutex);
}

static int starve_to_death(t_config *config, t_philo *philo)
{
	long now;

	now = get_timestamp_ms();
	if ((now - philo->last_meal) > config->time_to_die)
	{
		stop_simulation(config);
		pthread_mutex_lock(&config->print_mutex);
		printf("%ld %d died\n", now - config->start_time, philo->id);
		pthread_mutex_unlock(&config->print_mutex);
		return (1);
	}
	return (0);
}

static int all_philos_full(t_config *config, t_philo *philos)
{
	int i;

	if (config->eat_count == -1)
		return (0);
	i = 0;
	while (i < config->nb_philo)
	{
		if (philos[i].meals_eaten < config->eat_count)
			return (0);
		i++;
	}
	return (1);
}

void *monitor_function(void *arg)
{
	t_philo *philos;
	t_config *config;
	int i;

	philos = (t_philo *)arg;
	config = philos[0].config;
	while (!is_simulation_stopped(config))
	{
		i = 0;
		while (i < config->nb_philo && !is_simulation_stopped(config))
		{
			if (starve_to_death(config, &philos[i]))
				return (NULL);
			i++;
		}
		if (all_philos_full(config, philos))
			stop_simulation(config);
		usleep(1000);
	}
	return (NULL);
}
