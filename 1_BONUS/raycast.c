/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:57:43 by pboucher          #+#    #+#             */
/*   Updated: 2025/10/15 15:15:51 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

static void	has_touch(t_game *game)
{
	while (game->ray->hit == 0)
	{
		if (game->ray->sideDistX < game->ray->sideDistY)
		{
			game->ray->sideDistX += game->ray->deltaDistX;
			game->ray->mapX += game->ray->stepX;
			game->ray->side = 0;
		}
		else
		{
			game->ray->sideDistY += game->ray->deltaDistY;
			game->ray->mapY += game->ray->stepY;
			game->ray->side = 1;
		}
		if (game->tab[game->ray->mapX][game->ray->mapY] != '0')
			game->ray->hit = 1;
	}
}

static void	init_ray(t_game *game, int x)
{
	game->ray->cameraX = 2 * x / (float)WIDTH - 1;
	game->ray->rayDirX = game->ray->dirX + game->ray->planeX
		* game->ray->cameraX;
	game->ray->rayDirY = game->ray->dirY + game->ray->planeY
		* game->ray->cameraX;
	game->ray->mapX = (int)game->ray->posX;
	game->ray->mapY = (int)game->ray->posY;
	if (game->ray->rayDirX == 0)
		game->ray->deltaDistX = 1e30;
	else
		game->ray->deltaDistX = fabs(1.0f / game->ray->rayDirX);
	if (game->ray->rayDirY == 0)
		game->ray->deltaDistY = 1e30;
	else
		game->ray->deltaDistY = fabs(1.0f / game->ray->rayDirY);
	game->ray->hit = 0;
}

static void	update_step(t_ray *ray)
{
	ray->stepX = 1;
	ray->sideDistX = (ray->mapX + 1.0f - ray->posX) * ray->deltaDistX;
	if (ray->rayDirX < 0)
	{
		ray->stepX = -1;
		ray->sideDistX = (ray->posX - ray->mapX) * ray->deltaDistX;
	}
	ray->stepY = 1;
	ray->sideDistY = (ray->mapY + 1.0f - ray->posY) * ray->deltaDistY;
	if (ray->rayDirY < 0)
	{
		ray->stepY = -1;
		ray->sideDistY = (ray->posY - ray->mapY) * ray->deltaDistY;
	}
}

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
	static int h = 0;

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
			uint32_t colorF = (game->matrix->floor[game->ray->tx][game->ray->ty] + (h % 255 << 24 ) + (h % 255  << 16) + (h % 255 << 8));
			// uint32_t colorF = (game->matrix->floor[game->ray->tx][game->ray->ty]);
			mlx_put_pixel(game->screen, x, y, colorF);
			uint32_t colorC = (game->matrix->ceilling[game->ray->tx][game->ray->ty] + (h % 255 << 24 ) + (h % 255  << 16) + (h % 255 << 8));
			// uint32_t colorC = (game->matrix->ceilling[game->ray->tx][game->ray->ty]);
			mlx_put_pixel(game->screen, x, HEIGHT - y - 1, colorC);
			game->h = game->h + 1;
		}
	}
	h = h + 1;
	game->h = game->h + 1;
}

void	draw_ray(t_game *game)
{
	draw_floor_ceilling(game);
	game->ray->x = -1;
	while (++game->ray->x < WIDTH)
	{
		init_ray(game, game->ray->x);
		update_step(game->ray);
		has_touch(game);
		if (game->ray->side == 0)
			game->ray->perpWallDist = (game->ray->sideDistX
					- game->ray->deltaDistX);
		else
			game->ray->perpWallDist = (game->ray->sideDistY
					- game->ray->deltaDistY);
		game->ray->lineH = (int)(HEIGHT / game->ray->perpWallDist);
		game->ray->drawStart = -game->ray->lineH / 2 + HEIGHT / 2;
		if (game->ray->drawStart < 0)
			game->ray->drawStart = 0;
		game->ray->drawEnd = game->ray->lineH / 2 + HEIGHT / 2;
		if (game->ray->drawEnd >= HEIGHT)
			game->ray->drawEnd = HEIGHT - 1;
		draw_wall(game, game->ray->x);
	}
	game->h = game->h + 1 % 255;
}

void	detect_door(t_game *game)
{
	int	x;

	x = WIDTH / 2;
	init_ray(game, x);
	update_step(game->ray);
	has_touch(game);
	if (game->tab[game->ray->mapX][game->ray->mapY] == '2')
	{
		if ((game->ray->sideDistX - game->ray->deltaDistX) < 3
			&& (game->ray->sideDistY - game->ray->deltaDistY) < 3)
			game->tab[game->ray->mapX][game->ray->mapY] = '3';
	}
	else if (game->tab[game->ray->mapX][game->ray->mapY] == '3')
	{
		if ((game->ray->sideDistX - game->ray->deltaDistX) < 3
			&& (game->ray->sideDistY - game->ray->deltaDistY) < 3)
			game->tab[game->ray->mapX][game->ray->mapY] = '2';
	}
}
