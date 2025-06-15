/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:47:01 by pboucher          #+#    #+#             */
/*   Updated: 2025/06/15 17:01:34 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

void	move_player(t_game *game, float x, float y)
{
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT_SHIFT)
		|| mlx_is_key_down(game->mlx, MLX_KEY_RIGHT_SHIFT))
		change_speed(game, 1.5f, 1.5f);
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT_CONTROL)
		|| mlx_is_key_down(game->mlx, MLX_KEY_RIGHT_CONTROL))
		change_speed(game, 0.66f, 0.66f);
	if (!(int)(game->ray->posX + x * game->ray->moveSpeed)
		&& !game->tab[(int)game->ray->posX]
		[(int)(game->ray->posY + y * game->ray->moveSpeed)])
		return ;
	if (game->tab[(int)(game->ray->posX + x * game->ray->moveSpeed)]
		[(int)game->ray->posY] == '0')
		game->ray->posX += x * game->ray->moveSpeed;
	if (game->tab[(int)game->ray->posX]
		[(int)(game->ray->posY + y * game->ray->moveSpeed)] == '0')
		game->ray->posY += y * game->ray->moveSpeed;
}

void	rotate_cam(t_game *game, float rotSpeed)
{
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT_SHIFT)
		|| mlx_is_key_down(game->mlx, MLX_KEY_RIGHT_SHIFT))
		change_speed(game, 1.5f, 1.5f);
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT_CONTROL)
		|| mlx_is_key_down(game->mlx, MLX_KEY_RIGHT_CONTROL))
		change_speed(game, 0.66f, 0.66f);
	game->ray->oldDirX = game->ray->dirX;
	game->ray->dirX = result_x(game->ray->dirX, game->ray->dirY,
			rotSpeed);
	game->ray->dirY = result_y(game->ray->oldDirX, game->ray->dirY,
			rotSpeed);
	game->ray->oldPlaneX = game->ray->planeX;
	game->ray->planeX = result_x(game->ray->planeX, game->ray->planeY,
			rotSpeed);
	game->ray->planeY = result_y(game->ray->oldPlaneX, game->ray->planeY,
			rotSpeed);
}
