/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hganet <hganet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:49:48 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/19 15:14:04 by hganet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *philo_life(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg; // Cast the argument to t_philo pointer
	if (philo->config->nb_philo == 1) // If there's only one philosopher handle the case separatly to avoid deadlock (waiting for the right fork)
	{
		pthread_mutex_lock(philo->left_fork); // Lock the left fork
		log_action(philo, "has taken a fork"); // Safely log the action using print_mutex
		while (!is_simulation_stopped(philo->config)) // Check if the simulation is stopped
			usleep(1000); // Sleep for a short time to avoid busy waiting
		pthread_mutex_unlock(philo->left_fork); // Unlock the left fork
		return (NULL);
	}
	while (!is_simulation_stopped(philo->config)) // always check if the simulation is stopped
	{
		if (philo->config->eat_count != -1 && philo->meals_eaten >= philo->config->eat_count)
			break;
		take_forks(philo); // Philosopher takes both forks
		if (is_simulation_stopped(philo->config)) // Check if the simulation is stopped
			break; // If the simulation is stopped, exit the loop and return
		eat(philo); // log, update last meal, sleep and unlock forks
		if (is_simulation_stopped(philo->config)) // Check if the simulation is stopped
			break;
		sleep_and_think(philo); // log, sleep and think
	}
	return (NULL);
}
