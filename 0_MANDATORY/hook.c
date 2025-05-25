/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:35:40 by pboucher          #+#    #+#             */
/*   Updated: 2025/05/25 16:33:13 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int     need_refresh(t_game *game)
{
    if (mlx_is_key_down(game->mlx, UP) ||
        mlx_is_key_down(game->mlx, DOWN) ||
        mlx_is_key_down(game->mlx, LEFT) ||
        mlx_is_key_down(game->mlx, RIGHT) ||
        mlx_is_key_down(game->mlx, LEFT_R) ||
        mlx_is_key_down(game->mlx, RIGHT_R))
        return (1);
    return (0);
}

void    key_hook(t_game *game)
{
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
    if (need_refresh(game))
        refresh(game);
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
}
