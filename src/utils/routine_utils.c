/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hganet <hganet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:04:59 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/13 15:56:17 by hganet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void safe_sleep(t_config *config, long time_ms)
{
	long start;

	start = get_timestamp_ms();
	while (!is_simulation_stopped(config))
	{
		if (get_timestamp_ms() - start >= time_ms)
			break;
		usleep(500);
	}
}

void log_action(t_philo *philo, const char *msg)
{
	long ts;

	pthread_mutex_lock(&philo->config->print_mutex);
	ts = get_timestamp_ms() - philo->config->start_time;
	printf("%ld %d %s\n", ts, philo->id, msg);
	pthread_mutex_unlock(&philo->config->print_mutex);
}
void take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
		usleep(500);

	pthread_mutex_lock(philo->left_fork);
	log_action(philo, "has taken a fork");
	if (is_simulation_stopped(philo->config))
	{
		printf("simulation stopped");
		pthread_mutex_unlock(philo->left_fork);
		return;
	}
	pthread_mutex_lock(philo->right_fork);
	log_action(philo, "has taken a fork");
}

void eat(t_philo *philo)
{
	log_action(philo, "is eating");
	philo->last_meal = get_timestamp_ms();
	safe_sleep(philo->config, philo->config->time_to_eat);
	philo->meals_eaten++;

	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

void sleep_and_think(t_philo *philo)
{
	log_action(philo, "is sleeping");
	safe_sleep(philo->config, philo->config->time_to_sleep);
	log_action(philo, "is thinking");
}
