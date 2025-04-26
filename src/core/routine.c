/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:49:48 by hugoganet         #+#    #+#             */
/*   Updated: 2025/04/26 18:04:28 by hugoganet        ###   ########.fr       */
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
	t_philo *philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(500);
	while (!is_simulation_stopped(philo->config))
	{
		if (is_simulation_stopped(philo->config))
			break;
		pthread_mutex_lock(philo->left_fork);
		log_action(philo, "has taken a fork");

		if (is_simulation_stopped(philo->config))
		{
			pthread_mutex_unlock(philo->left_fork);
			break;
		}
		pthread_mutex_lock(philo->right_fork);
		log_action(philo, "has taken a fork");

		if (is_simulation_stopped(philo->config))
		{
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			break;
		}
		log_action(philo, "is eating");
		philo->last_meal = get_timestamp_ms();
		safe_sleep(philo->config, philo->config->time_to_eat);
		philo->meals_eaten++;

		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);

		if (is_simulation_stopped(philo->config))
			break;
		log_action(philo, "is sleeping");
		safe_sleep(philo->config, philo->config->time_to_sleep);

		if (is_simulation_stopped(philo->config))
			break;
		log_action(philo, "is thinking");
	}
	return (NULL);
}
