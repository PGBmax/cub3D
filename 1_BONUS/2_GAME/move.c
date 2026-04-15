/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:47:01 by pboucher          #+#    #+#             */
/*   Updated: 2026/04/15 15:09:27 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

static void	change_speed(t_game *game, float move, float rotate)
{
	game->ray->moveSpeed *= move;
	game->ray->rotSpeed *= rotate;
}

static float	result_x(float comp1, float comp2, float rotSpeed)
{
	return ((float)(comp1 * cosf(rotSpeed) - comp2 * sinf(rotSpeed)));
}

static float	result_y(float comp1, float comp2, float rotSpeed)
{
	return ((float)(comp1 * sinf(rotSpeed) + comp2 * cosf(rotSpeed)));
}

static int	is_walkable(char c)
{
	return (c == '0' || c == '3');
}

void	move_player(t_game *game, float x, float y)
{
	float	spd;
	float	nx;
	float	ny;
	float	m;

	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT_SHIFT))
		change_speed(game, 1.25f, 1.25f);
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT_CONTROL))
		change_speed(game, 0.8f, 0.8f);
	spd = game->ray->moveSpeed;
	m = WALL_MARGIN;
	nx = game->ray->posX + x * spd;
	ny = game->ray->posY + y * spd;
	if (is_walkable(game->tab[(int)(nx + m)][(int)game->ray->posY])
		&& is_walkable(game->tab[(int)(nx - m)][(int)game->ray->posY]))
		game->ray->posX = nx;
	if (is_walkable(game->tab[(int)game->ray->posX][(int)(ny + m)])
		&& is_walkable(game->tab[(int)game->ray->posX][(int)(ny - m)]))
		game->ray->posY = ny;
}

void	rotate_cam(t_game *game, float rotSpeed)
{
	game->ray->oldDirX = game->ray->dirX;
	game->ray->dirX = result_x(game->ray->dirX, game->ray->dirY, rotSpeed);
	game->ray->dirY = result_y(game->ray->oldDirX, game->ray->dirY, rotSpeed);
	game->ray->oldPlaneX = game->ray->planeX;
	game->ray->planeX = result_x(game->ray->planeX, game->ray->planeY,
			rotSpeed);
	game->ray->planeY = result_y(game->ray->oldPlaneX, game->ray->planeY,
			rotSpeed);
}
