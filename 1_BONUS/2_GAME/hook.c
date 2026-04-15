/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:35:40 by pboucher          #+#    #+#             */
/*   Updated: 2026/04/15 14:58:32 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

static void	update_fov(t_game *game)
{
	float	len;
	float	ratio;

	game->cur_fov += (game->target_fov - game->cur_fov) * FOV_LERP;
	len = sqrtf(game->ray->planeX * game->ray->planeX
			+ game->ray->planeY * game->ray->planeY);
	if (len < 0.001f)
		return ;
	ratio = game->cur_fov / len;
	game->ray->planeX *= ratio;
	game->ray->planeY *= ratio;
}

void	key_hook(t_game *game)
{
	refresh(game);
	game->is_moving = 0;
	game->target_fov = FOV;
	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(game->mlx);
	if (!game->paused && mlx_is_key_down(game->mlx, UP))
	{
		move_player(game, game->ray->dirX, game->ray->dirY);
		game->is_moving = 1;
	}
	if (!game->paused && mlx_is_key_down(game->mlx, DOWN))
	{
		move_player(game, -game->ray->dirX, -game->ray->dirY);
		game->is_moving = 1;
	}
	if (!game->paused && mlx_is_key_down(game->mlx, RIGHT))
	{
		move_player(game, game->ray->planeX, game->ray->planeY);
		game->is_moving = 1;
	}
	if (!game->paused && mlx_is_key_down(game->mlx, LEFT))
	{
		move_player(game, -game->ray->planeX, -game->ray->planeY);
		game->is_moving = 1;
	}
	if (!game->paused && mlx_is_key_down(game->mlx, RIGHT_R))
		rotate_cam(game, -game->ray->rotSpeed);
	if (!game->paused && mlx_is_key_down(game->mlx, LEFT_R))
		rotate_cam(game, game->ray->rotSpeed);
	if (!game->paused && mlx_is_key_down(game->mlx, MLX_KEY_LEFT_SHIFT)
		&& game->is_moving)
		game->target_fov = FOV + SPRINT_FOV_ADD;
	update_fov(game);
}

void	cursor_hook(t_game *game)
{
	int		x;
	int		y;
	int		w;
	int		h;
	float	raw;

	if (!game->paused)
	{
		mlx_set_cursor_mode(game->mlx, MLX_MOUSE_HIDDEN);
		mlx_get_mouse_pos(game->mlx, &x, &y);
		w = game->mlx->width;
		h = game->mlx->height;
		raw = (float)(x - w / 2) * MOUSE_SENS;
		game->smooth_rot += (raw - game->smooth_rot) * MOUSE_SMOOTH;
		rotate_cam(game, -game->smooth_rot);
		mlx_set_mouse_pos(game->mlx, w / 2, h / 2);
	}
	else
		mlx_set_cursor_mode(game->mlx, MLX_MOUSE_NORMAL);
}

void	game_pause(mlx_key_data_t key_data, t_game *game)
{
	if ((key_data.action != MLX_PRESS) && key_data.key != MLX_KEY_ESCAPE)
		return ;
	if (key_data.key == MLX_KEY_P)
	{
		if (game->paused)
		{
			game->sprite->pause->enabled = false;
			game->paused = 0;
		}
		else if (!game->paused)
		{
			game->sprite->pause->enabled = true;
			game->paused = 1;
		}
	}
	if (key_data.key == MLX_KEY_E)
		detect_door(game);
}
