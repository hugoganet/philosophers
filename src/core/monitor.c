/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 09:07:24 by hugoganet         #+#    #+#             */
/*   Updated: 2025/04/26 15:55:04 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Safely check if someone has died by locking the death mutex.
 *
 * @param config Shared configuration with flag and mutex.
 * @return int Returns 1 if a death occurred.
 */
static int check_if_dead(t_config *config)
{
	int ret;

	pthread_mutex_lock(&config->death_mutex);
	ret = config->someone_died;
	pthread_mutex_unlock(&config->death_mutex);
	return (ret);
}

/**
 * @brief Monitors philosophers for death.
 *
 * Runs in its own thread, checks each philosopher's last_meal
 * and stops simulation if any exceeds time_to_die.
 *
 * @param arg The array of philosophers (cast from void*).
 * @return void* Always returns NULL.
 */
void *death_monitor(void *arg)
{
	t_philo *philos = (t_philo *)arg;
	t_config *config = philos[0].config;
	int i;
	long now;

	while (!check_if_dead(config))
	{
		i = 0;
		while (i < config->nb_philo && !check_if_dead(config))
		{
			now = get_timestamp_ms();
			if (now - (philos[i].last_meal) > config->time_to_die)
			{
				pthread_mutex_lock(&config->death_mutex);
				config->someone_died = 1;
				pthread_mutex_unlock(&config->death_mutex);
				pthread_mutex_lock(&config->print_mutex);
				printf("%ld %d died\n", now - config->start_time, philos[i].id);
				pthread_mutex_unlock(&config->print_mutex);
			}
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}