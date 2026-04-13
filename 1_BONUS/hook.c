/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:35:40 by pboucher          #+#    #+#             */
/*   Updated: 2025/08/16 08:34:25 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	key_hook(t_game *game)
{
	refresh(game);
	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(game->mlx);
	if (!game->paused && mlx_is_key_down(game->mlx, UP))
		move_player(game, game->ray->dirX, game->ray->dirY);
	if (!game->paused && mlx_is_key_down(game->mlx, DOWN))
		move_player(game, -game->ray->dirX, -game->ray->dirY);
	if (!game->paused && mlx_is_key_down(game->mlx, RIGHT))
		move_player(game, game->ray->planeX, game->ray->planeY);
	if (!game->paused && mlx_is_key_down(game->mlx, LEFT))
		move_player(game, -game->ray->planeX, -game->ray->planeY);
	if (!game->paused && mlx_is_key_down(game->mlx, RIGHT_R))
		rotate_cam(game, -game->ray->rotSpeed);
	if (!game->paused && mlx_is_key_down(game->mlx, LEFT_R))
		rotate_cam(game, game->ray->rotSpeed);
}

void	cursor_hook(t_game *game)
{
    static float last_rot = 0.0f;
    int		x, y;
    int		w, h;
    float	rot;
    const float sensitivity = 0.0025f;
    const float smoothing = 0.6f; // Entre 0 (pas de lissage) et 1 (lissage fort)

    if (!game->paused)
    {
        mlx_set_cursor_mode(game->mlx, MLX_MOUSE_HIDDEN);
        mlx_get_mouse_pos(game->mlx, &x, &y);
        w = game->mlx->width;
        h = game->mlx->height;
        rot = (float)(x - w / 2) * sensitivity;
        // Lissage de la rotation
        rot = last_rot * smoothing + rot * (1.0f - smoothing);
        last_rot = rot;
        rotate_cam(game, -rot);
        mlx_set_mouse_pos(game->mlx, w / 2, h / 2);
    }
    else
    {
        mlx_set_cursor_mode(game->mlx, MLX_MOUSE_NORMAL);
    }
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
