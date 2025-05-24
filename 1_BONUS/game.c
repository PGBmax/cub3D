/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:40:15 by pboucher          #+#    #+#             */
/*   Updated: 2025/05/24 17:11:30 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"
#include <stdio.h>

uint32_t    rgb_to_hex32(int *rgb)
{
    return ((uint32_t)((rgb[0] << 24) | (rgb[1] << 16) | (rgb[2] << 8) | 255));
}

void draw_map(t_game *game)
{
    int y;
    int x;
    
    y = 0;
    while (game->tab[y])
    {
        x = 0;
        while (game->tab[y][x + 1])
        {
            if (game->tab[y][x] == game->info->pos)
			{
                game->player->x = x;
				game->player->y = y;
				game->tab[y][x] = '0';
			}
            x++;
        }
        y++;
    }
}

void refresh(t_game *game)
{
    static int i = -1;

    int j;

    j = -1;
    while (++j < 44)
        game->sprite->frames[j]->enabled = false;
    ++i;
    i = i % 44;
    game->sprite->frames[i]->enabled = true;
    draw_ray(game);
    game->r->moveSpeed = MOVESPD;
    game->r->rotSpeed = ROTSPD;
}

int ft_game(t_game *game)
{
    mlx_image_t     *background;

    mlx_set_setting(MLX_STRETCH_IMAGE, true);
    game->mlx = mlx_init(WIDTH, HEIGHT, "cub3D", true);
    if (!game->mlx)
    {
        free(game->info);
        error_msg(MLX_CANNOT_CREATE, NULL);
    }
    background = mlx_new_image(game->mlx, 1, 1);
    mlx_put_pixel(background, 0, 0, 0x000000FF);
    mlx_resize_image(background, WIDTH, HEIGHT);
    mlx_image_to_window(game->mlx, background, 0 , 0);
    game->screen = mlx_new_image(game->mlx, WIDTH, HEIGHT);
    mlx_image_to_window(game->mlx, game->screen, 0, 0);
    game->player = ft_calloc(sizeof(t_player), 1);
    draw_map(game);
    if (!game_init(game))
        return (0);
    mlx_key_hook(game->mlx, (void (*))game_pause, (void *)game);
    mlx_loop_hook(game->mlx, (void (*))key_hook, (void *)game);
    mlx_cursor_hook(game->mlx, (void (*))cursor_hook, (void *)game);
    mlx_loop(game->mlx);
    mlx_terminate(game->mlx);
    return (1);
}
