/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 14:34:23 by pboucher          #+#    #+#             */
/*   Updated: 2025/05/28 14:34:23 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	draw_each_line(t_game *game, t_fc *fc)
{
	while (++fc->x < WIDTH)
	{
		fc->cellX = (int)fc->floorX;
		fc->cellY = (int)fc->floorY;
		fc->tx = (int)(game->s_width * (fc->floorX - fc->cellX))
			& (game->s_width - 1);
		fc->ty = (int)(game->s_height * (fc->floorY - fc->cellY))
			& (game->s_height - 1);
		fc->floorX += fc->floorStepX;
		fc->floorY += fc->floorStepY;
		fc->color;
		fc->color = get_color(game->sprite->floor, fc->tx, fc->ty);
		mlx_put_pixel(game->screen, fc->x, fc->y, fc->color);
		fc->color = get_color(game->sprite->ceilling, fc->tx, fc->ty);
		mlx_put_pixel(game->screen, fc->x, HEIGHT - fc->y - 1, fc->color);
	}
}

void	draw_floor_ceilling(t_game *game)
{
	t_fc	fc;

	fc.y = -1;
	while (++fc.y < HEIGHT)
	{
		x = -1;
		fc.rayDirX0 = game->r->dirX - game->r->planeX;
		fc.rayDirY0 = game->r->dirY - game->r->planeY;
		fc.rayDirX1 = game->r->dirX + game->r->planeX;
		fc.rayDirY1 = game->r->dirY + game->r->planeY;
		fc.p = fc.y - HEIGHT / 2;
		fc.posZ = 0.5 * HEIGHT;
		fc.rowDistance = fc.posZ / fc.p;
		fc.floorStepX = fc.rowDistance * (fc.rayDirX1 - fc.rayDirX0) / WIDTH;
		fc.floorStepY = fc.rowDistance * (fc.rayDirY1 - fc.rayDirY0) / WIDTH;
		fc.floorX = game->r->posX + fc.rowDistance * fc.rayDirX0;
		fc.floorY = game->r->posY + fc.rowDistance * fc.rayDirY0;
		draw_each_line(game, &fc);
	}
}
