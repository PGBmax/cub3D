/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:35:40 by pboucher          #+#    #+#             */
/*   Updated: 2025/06/09 16:42:02 by pboucher         ###   ########.fr       */
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
    if (need_refresh(game))
        refresh(game);
}
