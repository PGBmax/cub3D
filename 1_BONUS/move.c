/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:47:01 by pboucher          #+#    #+#             */
/*   Updated: 2025/05/28 14:19:31 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

static void	change_speed(t_game *game, float move, float rotate)
{
	game->r->moveSpeed *= move;
	game->r->rotSpeed *= rotate;
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
	if (!(int)(game->r->posX + x * game->r->moveSpeed) &&
		!game->tab[(int)game->r->posX]
		[(int)(game->r->posY + y * game->r->moveSpeed)])
		return ;
	if (game->tab[(int)(game->r->posX + x * game->r->moveSpeed)]
		[(int)game->r->posY] == '0')
		game->r->posX += x * game->r->moveSpeed;
	if (game->tab[(int)game->r->posX]
		[(int)(game->r->posY + y * game->r->moveSpeed)] == '0')
		game->r->posY += y * game->r->moveSpeed;
	if (game->tab[(int)(game->r->posX + x * game->r->moveSpeed)]
		[(int)game->r->posY] == '3')
		game->r->posX += x * game->r->moveSpeed;
	if (game->tab[(int)game->r->posX]
		[(int)(game->r->posY + y * game->r->moveSpeed)] == '3')
		game->r->posY += y * game->r->moveSpeed;
}

void	rotate_cam(t_game *game, float rotSpeed)
{
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT_SHIFT)
		|| mlx_is_key_down(game->mlx, MLX_KEY_RIGHT_SHIFT))
		change_speed(game, 1.5f, 1.5f);
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT_CONTROL)
		|| mlx_is_key_down(game->mlx, MLX_KEY_RIGHT_CONTROL))
		change_speed(game, 0.66f, 0.66f);
	game->r->oldDirX = game->r->dirX;
	game->r->dirX = result_x(game->r->dirX, game->r->dirY, rotSpeed);
	game->r->dirY = result_y(game->r->oldDirX, game->r->dirY, rotSpeed);
	game->r->oldPlaneX = game->r->planeX;
	game->r->planeX = result_x(game->r->planeX, game->r->planeY, rotSpeed);
	game->r->planeY = result_y(game->r->oldPlaneX, game->r->planeY, rotSpeed);
}
