/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:35:40 by pboucher          #+#    #+#             */
/*   Updated: 2025/06/08 18:00:18 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void    key_hook(t_game *game)
{
    refresh(game);
    if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
        mlx_close_window(game->mlx);
    if (!game->paused && mlx_is_key_down(game->mlx, UP))
        move_player(game, game->r->dirX, game->r->dirY);
    if (!game->paused && mlx_is_key_down(game->mlx, DOWN))
        move_player(game, -game->r->dirX, -game->r->dirY);
    if (!game->paused && mlx_is_key_down(game->mlx, RIGHT))
        move_player(game, game->r->planeX, game->r->planeY);
    if (!game->paused && mlx_is_key_down(game->mlx, LEFT))
        move_player(game, -game->r->planeX, -game->r->planeY);
    if (!game->paused && mlx_is_key_down(game->mlx, RIGHT_R))
        rotate_cam(game, -game->r->rotSpeed);
    if (!game->paused && mlx_is_key_down(game->mlx, LEFT_R))
        rotate_cam(game, game->r->rotSpeed);
}

void    cursor_hook(t_game *game)
{
    static int x = {WIDTH / 2};
    static int y = {HEIGHT / 2};
    int w;
    int h;
    float rot;

	if (!game->paused)
	{
		rot = ROTSPD;
		mlx_set_cursor_mode(game->mlx, MLX_MOUSE_HIDDEN);
		rot = rot * (x - game->mlx->width / 2) * 0.01f;
		mlx_get_mouse_pos(game->mlx, &x, &y);
        w = game->mlx->width;
        h = game->mlx->height;
        rotate_cam(game, -rot);
		mlx_set_mouse_pos(game->mlx, w / 2, h / 2);
	}
    if (game->paused)
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
