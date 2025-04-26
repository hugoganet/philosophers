/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:03:08 by hugoganet         #+#    #+#             */
/*   Updated: 2025/04/26 15:46:20 by hugoganet        ###   ########.fr       */
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

	if (parse_args(argc, argv, &config))
		return (printf("Error: invalid arguments\n"), 1);
	if (init_simulation(&config, &philos, &forks))
		return (printf("Error: initialization failed\n"), 1);
	if (launch_threads(&config, philos))
		return (printf("Error: thread creation failed\n"), 1);
	if (pthread_create(&monitor, NULL, death_monitor, philos) != 0)
		return (printf("Error: failed to launch monitor\n"), 1);

	// 🔁 Wait for monitor to finish first
	pthread_join(monitor, NULL);

	// 🔁 Wait for all philosopher threads to finish
	i = 0;
	while (i < config.nb_philo)
		pthread_join(philos[i++].thread, NULL);

	cleanup_simulation(&config, philos, forks);
	return (0);
}

int launch_threads(t_config *config, t_philo *philos)
{
	int i;

	i = 0;
	while (i < config->nb_philo)
	{
		/**
		 * The pthread_create function is used to create a new thread in a POSIX-compliant system.
		 * It takes a pointer to a thread identifier, optional thread attributes,
		 * a function pointer for the thread's start routine (which cannot be null), 
		 * and an optional argument to pass to the start routine.
		 */
		if (pthread_create(&philos[i].thread, NULL, philo_life, &philos[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}
