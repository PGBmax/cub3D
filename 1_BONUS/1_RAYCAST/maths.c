/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 13:33:48 by maregnie          #+#    #+#             */
/*   Updated: 2025/06/15 15:55:58 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

t_pos	adding_pos(t_pos a, t_pos b)
{
	t_pos	c;

	c.x = a.x + b.x;
	c.y = a.y + b.y;
	return (c);
}

float	calc_len(t_pos pos)
{
	float	len;

	len = sqrt(pow(pos.x, 2) + pow(pos.y, 2));
	return (len);
}

float	calc_dist(t_pos a, t_pos b)
{
	t_pos	new;

	new.x = a.x - b.x;
	new.y = a.y - b.y;
	return (calc_len(new));
}

int	in_circle(t_pos player, t_pos center, float radius)
{
	if (calc_dist(player, center) < radius)
		return (1);
	return (0);
}

t_pos	newpos(float x, float y)
{
	t_pos	pos;

	pos.x = x;
	pos.y = y;
	return (pos);
}
