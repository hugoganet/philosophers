/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:52:11 by hugoganet         #+#    #+#             */
/*   Updated: 2025/04/26 15:49:45 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>

long get_timestamp_ms(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000L) + (tv.tv_usec / 1000L));
}

long timeval_to_ms(struct timeval tv)
{
	return ((tv.tv_sec * 1000L) + (tv.tv_usec / 1000L));
}
