/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hganet <hganet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:47:04 by hganet            #+#    #+#             */
/*   Updated: 2025/05/13 12:51:42 by hganet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int is_simulation_stopped(t_config *config)
{
	int ret;

	pthread_mutex_lock(&config->death_mutex); // Lock the mutex to safely access shared data
	ret = config->stop_simulation; // Check if the simulation flag is set to 1 (indicating stop)
	// If the simulation is stopped, we can safely unlock the mutex and return
	pthread_mutex_unlock(&config->death_mutex);
	return (ret);
}

static void stop_simulation(t_config *config)
{
	pthread_mutex_lock(&config->death_mutex); // Lock the mutex to safely modify shared data
	config->stop_simulation = 1; // Set the stop_simulation flag to 1 (indicating stop)
	pthread_mutex_unlock(&config->death_mutex); // Unlock the mutex after modifying the flag
}

static int starve_to_death(t_config *config, t_philo *philo)
{
	long now;

	now = get_timestamp_ms(); // Get the current timestamp in milliseconds
	if ((now - philo->last_meal) > config->time_to_die) 
	{
		stop_simulation(config); // Stop the simulation if a philosopher has starved by setting the flag to 1
		pthread_mutex_lock(&config->print_mutex); // Lock the print mutex to safely print the message
		printf("%ld %d died\n", now - config->start_time, philo->id); // Print the death message with timestamp and philosopher ID
		pthread_mutex_unlock(&config->print_mutex); // Unlock the print mutex after printing
		return (1); // Return 1 to indicate that the philosopher has starved to death
	}
	return (0);
}

static int all_philos_full(t_config *config, t_philo *philos)
{
	int i;

	if (config->eat_count == -1) // If the eat_count is -1, we don't need to check if all philosophers are full because the argument is not set
		return (0);
	i = 0;
	while (i < config->nb_philo) // iterate through all philosophers
	{
		if (philos[i].meals_eaten < config->eat_count) // Check if any philosopher has not eaten enough times
			// If any philosopher has not eaten enough times, we return 0 to indicate that not all philosophers are full
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

	philos = (t_philo *)arg; // Cast the argument to philosopher array
	if (!philos) // Check if the argument is NULL to avoid dereferencing a NULL pointer
		return (NULL);
	config = philos[0].config; // Get the config from the first philosopher
	while (!is_simulation_stopped(config)) // Check stop_simulation flag
	{
		i = 0;
		while (i < config->nb_philo && !is_simulation_stopped(config)) // Check if simulation is stopped before checking each philosopher
		{
			if (starve_to_death(config, &philos[i])) // Check if the philosopher has starved to death
				return (NULL);
			i++;
		}
		if (all_philos_full(config, philos)) // Check if all philosophers have eaten enough times
			stop_simulation(config); // Stop the simulation by setting the stop_simulation to 1
		usleep(1000); // Sleep for a short duration to avoid surcharging the CPU
	}
	return (NULL);
}
