/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_threads.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hganet <hganet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 18:23:30 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/13 12:26:31 by hganet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int launch_threads(t_config *config, t_philo *philos)
{
	int i;

	i = 0;
	while (i < config->nb_philo) // Iterate through each philosopher	
	{
		/**
		 * Create a thread for each philosopher.
		 * The thread will execute the philo_life function(_start_routine) with the philosopher's struct as an argument(arg).
		 * The thread is created in a detached state, meaning it will clean up its resources automatically when it terminates.
		 * The thread's ID is stored in the philosopher's struct. It's used to join the thread later.
		 */
		if (pthread_create(&philos[i].thread, NULL, philo_life, &philos[i]) != 0)
			return (0);
		i++;
	}
	return (1);
}