/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hganet <hganet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:03:08 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/19 15:18:31 by hganet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int main(int argc, char **argv)
{
	t_config config;
	t_philo *philos; // array of t_philo
	pthread_mutex_t *forks;
	pthread_t monitor;
	int i;

	if (!parse_args(argc, argv, &config)) // parse_args function checks the arguments and fills the config struct
		return (printf("Error: invalid arguments\n"), 1);
	if (!init_simulation(&config, &philos, &forks)) // Allocate memory for philosophers and forks, initiate mutexes (forks, print and death)
		return (printf("Error: initialization failed\n"), 1);
	if (!launch_threads(&config, philos)) // Create threads for each philosopher and store their IDs in philos[i].thread
		return (printf("Error: thread creation failed\n"), 1);
	if (pthread_create(&monitor, NULL, monitor_function, philos) != 0) // Create a thread for the monitor_function and store its ID in monitor variable
		return (printf("Error: failed to launch monitor_function\n"), 1);

	// 🔁 Wait for monitor_function to finish first
	pthread_join(monitor, NULL);

	// 🔁 Wait for all philosopher threads to finish
	i = 0;
	while (i < config.nb_philo)
		pthread_join(philos[i++].thread, NULL);
	// print_meals_count(&philos, config);
	cleanup_simulation(&config, philos, forks); // Free allocated memory and destroy mutexes
	return (0);
}
