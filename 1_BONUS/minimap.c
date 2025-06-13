/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:39:23 by maregnie          #+#    #+#             */
/*   Updated: 2025/06/13 14:26:02 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"
#include "stdio.h"

static double	clamp(double x, double min, double max)
{
	if (x < min)
		return (min);
	if (x > max)
		return (max);
	return (x);
}

t_color	set_color(t_byte r, t_byte g, t_byte b, t_byte a)
{
	t_color	new;

	new.red = r;
	new.green = g;
	new.blue = b;
	new.opacity = a;
	new.color = (r << 24 | g << 16 | b << 8 | a);
	return (new);
}

t_color	pick_color(t_game *game, t_pos loop)
{
	t_color	color;

	color = set_color(0, 0, 0, 0);
	if (in_circle(loop, newpos(0, 0), 2))
		color = set_color(255, 0, 0, 255);
	else if (in_circle(loop, newpos(0, 0), 50 - 2))
	{
		if (minimap(game, loop))
			color = set_color(0, 0, 0, 255);
		else if (!minimap(game, loop))
			color = set_color(180, 180, 180, 255);
	}
	else if (in_circle(loop, newpos(0, 0), 50))
		color = set_color(128, 128, 128, 128);
	return (color);
}

void	draw_minimap(t_game *game, t_color color, t_pos pos)
{
	if (!game->screen)
		return ;
	if (color.color == 0)
		return ;
	if (pos.y == game->screen->height)
		pos.y--;
	if (pos.x < 0 || pos.x > game->screen->width
		|| pos.y < 0 || pos.y > game->screen->height)
		return ;
	mlx_put_pixel(game->screen, pos.x, pos.y, color.color);
}

uint32_t	minimap(t_game *game, t_pos pos)
{
	double	x;
	double	y;
	double	px;
	double	py;

	py = game->ray->posX;
	px = game->ray->posY;
	y = clamp((py + pos.y / 16.5f), 0, game->screen->height);
	if (y < 0)
		y = 0;
	if (y > game->screen->height)
		y = game->screen->height;
	x = clamp((px + pos.x / 15), 0, ft_strlen(game->tab[(int)y]));
	if (game->tab && game->tab[(int)y] && game->tab[(int)y][(int)x]
		&& (game->tab[(int)y][(int)x] == '0'
		|| game->tab[(int)y][(int)x] == '3'
		|| game->tab[(int)y][(int)x] == 'N'
		|| game->tab[(int)y][(int)x] == 'S'
		|| game->tab[(int)y][(int)x] == 'E'
		|| game->tab[(int)y][(int)x] == 'W'))
		return (0);
	return (1);
}
