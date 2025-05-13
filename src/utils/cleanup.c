/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hganet <hganet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 18:10:15 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/13 15:53:54 by hganet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void cleanup_simulation(t_config *config, t_philo *philos, pthread_mutex_t *forks)
{
	int i;

	if (forks) // Check if forks are allocated to avoid double free
	{
		i = 0;
		while (i < config->nb_philo) // iterate through each philosopher
			pthread_mutex_destroy(&forks[i++]); // Destroy each fork mutex
		free(forks); // Free the forks array
	}
	pthread_mutex_destroy(&config->print_mutex); // Destroy the print mutex
	pthread_mutex_destroy(&config->death_mutex); // Destroy the death mutex
	free(philos); // Free the philosophers array
}