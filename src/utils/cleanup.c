/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 18:10:15 by hugoganet         #+#    #+#             */
/*   Updated: 2025/04/25 11:35:31 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void cleanup_simulation(t_config *config, t_philo *philos, pthread_mutex_t *forks)
{
	int i;

	if (forks)
	{
		i = 0;
		while (i < config->nb_philo)
			pthread_mutex_destroy(&forks[i++]);
		free(forks);
	}
	pthread_mutex_destroy(&config->print_mutex);
	pthread_mutex_destroy(&config->death_mutex);
	free(philos);
}