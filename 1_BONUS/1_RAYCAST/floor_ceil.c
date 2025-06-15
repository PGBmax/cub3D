/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor_ceil.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 15:53:55 by pboucher          #+#    #+#             */
/*   Updated: 2025/06/15 16:13:15 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	calculate_floor_ceilling(t_game *game, int y)
{
	game->ray->rayDirX0 = game->ray->dirX - game->ray->planeX;
	game->ray->rayDirY0 = game->ray->dirY - game->ray->planeY;
	game->ray->rayDirX1 = game->ray->dirX + game->ray->planeX;
	game->ray->rayDirY1 = game->ray->dirY + game->ray->planeY;
	game->ray->p = y - HEIGHT / 2;
	game->ray->posZ = 0.5 * HEIGHT;
	game->ray->rowDistance = game->ray->posZ / game->ray->p;
	game->ray->floorStepX = game->ray->rowDistance
		* (game->ray->rayDirX1 - game->ray->rayDirX0) / WIDTH;
	game->ray->floorStepY = game->ray->rowDistance
		* (game->ray->rayDirY1 - game->ray->rayDirY0) / WIDTH;
	game->ray->floorX = game->ray->posX + game->ray->rowDistance
		* game->ray->rayDirX0;
	game->ray->floorY = game->ray->posY + game->ray->rowDistance
		* game->ray->rayDirY0;
}

void	draw_floor_ceilling(t_game *game)
{
	int	x;
	int	y;

	y = -1;
	while (++y < HEIGHT)
	{
		x = -1;
		calculate_floor_ceilling(game, y);
		while (++x < WIDTH)
		{
			game->ray->cellX = (int)game->ray->floorX;
			game->ray->cellY = (int)game->ray->floorY;
			game->ray->tx = (int)(S_BOX
					* (game->ray->floorX - game->ray->cellX)) & (S_BOX - 1);
			game->ray->ty = (int)(S_BOX
					* (game->ray->floorY - game->ray->cellY)) & (S_BOX - 1);
			game->ray->floorX += game->ray->floorStepX;
			game->ray->floorY += game->ray->floorStepY;
			mlx_put_pixel(game->screen, x, y,
				game->matrix->floor[game->ray->tx][game->ray->ty]);
			mlx_put_pixel(game->screen, x, HEIGHT - y - 1,
				game->matrix->ceilling[game->ray->tx][game->ray->ty]);
		}
	}
}
