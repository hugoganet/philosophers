/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:49:48 by hugoganet         #+#    #+#             */
/*   Updated: 2025/04/26 18:34:53 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *philo_life(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	if (philo->config->nb_philo == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		log_action(philo, "has taken a fork");
		while (!is_simulation_stopped(philo->config))
			usleep(1000);
		pthread_mutex_unlock(philo->left_fork);
		return (NULL);
	}
	while (!is_simulation_stopped(philo->config))
	{
		take_forks(philo);
		if (is_simulation_stopped(philo->config))
			break;
		eat(philo);
		if (is_simulation_stopped(philo->config))
			break;
		sleep_and_think(philo);
	}
	return (NULL);
}