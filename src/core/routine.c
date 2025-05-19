/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hganet <hganet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:49:48 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/19 17:13:35 by hganet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	handle_one_philo_case(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	log_action(philo, "has taken a fork");
	while (!is_simulation_stopped(philo->config))
		usleep(1000);
	pthread_mutex_unlock(philo->left_fork);
}

void	*philo_life(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->config->nb_philo == 1)
	{
		handle_one_philo_case(philo);
		return (NULL);
	}
	while (!is_simulation_stopped(philo->config))
	{
		if (philo->config->eat_count != -1
			&& philo->meals_eaten >= philo->config->eat_count
			&& is_simulation_stopped(philo->config))
			break ;
		take_forks(philo);
		if (is_simulation_stopped(philo->config))
			break ;
		eat(philo);
		if (is_simulation_stopped(philo->config))
			break ;
		sleep_and_think(philo);
	}
	return (NULL);
}
