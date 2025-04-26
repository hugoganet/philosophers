/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:49:48 by hugoganet         #+#    #+#             */
/*   Updated: 2025/04/26 17:30:19 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Logs a message with timestamp and philosopher ID.
 *
 * This function uses a shared mutex to ensure print output
 * is synchronized between threads.
 *
 * @param philo Philosopher pointer.
 * @param msg The message to log.
 */
static void log_action(t_philo *philo, const char *msg)
{
	long ts;

	pthread_mutex_lock(&philo->config->print_mutex);
	ts = get_timestamp_ms() - philo->config->start_time;
	printf("%ld %d %s\n", ts, philo->id, msg);
	pthread_mutex_unlock(&philo->config->print_mutex);
}

void *philo_life(void *arg)
{
	t_philo *philo = (t_philo *)arg;

	while (!is_simulation_stopped(philo->config))
	{
		pthread_mutex_lock(philo->left_fork);
		log_action(philo, "has taken a fork");

		if (pthread_mutex_lock(philo->right_fork) != 0)
		{
			pthread_mutex_unlock(philo->left_fork);
			continue;
		}
		log_action(philo, "has taken a fork");

		// EAT
		philo->last_meal = get_timestamp_ms();
		log_action(philo, "is eating");
		safe_sleep(philo->config, philo->config->time_to_eat);
		philo->meals_eaten++;

		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);

		log_action(philo, "is sleeping");
		safe_sleep(philo->config, philo->config->time_to_sleep);

		log_action(philo, "is thinking");
	}

	return (NULL);
}