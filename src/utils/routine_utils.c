/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:04:59 by hugoganet         #+#    #+#             */
/*   Updated: 2025/04/26 17:07:07 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void safe_sleep(t_config *config, long time_ms)
{
	long start;

	start = get_timestamp_ms();
	while (!is_simulation_stopped(config))
	{
		if (get_timestamp_ms() - start >= time_ms)
			break;
		usleep(500);
	}
}