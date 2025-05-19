/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hganet <hganet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:04:59 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/19 15:17:11 by hganet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void safe_sleep(t_config *config, long time_to_eat)
{
	long start;

	start = get_timestamp_ms(); // Get the current timestamp in milliseconds
	while (!is_simulation_stopped(config)) // Check if the simulation is stopped
	{
		if (get_timestamp_ms() - start >= time_to_eat) // check if the sleep time has elapsed
			break;
		usleep(500); // Sleep for a short duration 
	}
}


void log_action(t_philo *philo, const char *msg)
{
	long ts;

	pthread_mutex_lock(&philo->config->print_mutex); // Lock the print mutex to prevent overlapping outputs
	ts = get_timestamp_ms() - philo->config->start_time; // Get the current timestamp
	printf("%ld %d %s\n", ts, philo->id, msg); // Print the message with timestamp and philosopher ID
	pthread_mutex_unlock(&philo->config->print_mutex); // Unlock the print mutex after printing
}

void take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0) // Even philosophers take the left fork first
		usleep(500);
	pthread_mutex_lock(philo->left_fork); // Lock the left fork
	log_action(philo, "has taken a fork"); // Safely log the action using print_mutex
	if (is_simulation_stopped(philo->config)) // Check if the simulation is stopped	
	{
		pthread_mutex_unlock(philo->left_fork); // Unlock the left fork
		return ; // Exit the function if the simulation is stopped
	}
	pthread_mutex_lock(philo->right_fork); // Lock the right fork
	log_action(philo, "has taken a fork"); // Safely log the action using print_mutex
}

void eat(t_philo *philo)
{
	log_action(philo, "is eating"); // Safely log the action using print_mutex
	philo->last_meal = get_timestamp_ms(); // Update the last meal timestamp
	safe_sleep(philo->config, philo->config->time_to_eat); // Sleep for the configured eating duration
	philo->meals_eaten++; // Increment the meal counter

	pthread_mutex_unlock(philo->right_fork); // Unlock the right fork
	pthread_mutex_unlock(philo->left_fork); // Unlock the left fork
}

void sleep_and_think(t_philo *philo)
{
	log_action(philo, "is sleeping");
	safe_sleep(philo->config, philo->config->time_to_sleep);
	log_action(philo, "is thinking");
}
