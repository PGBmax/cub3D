/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 15:14:23 by pboucher          #+#    #+#             */
/*   Updated: 2025/05/23 16:28:57 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

uint32_t get_color(mlx_image_t *img, int x, int y)
{
    return ((uint32_t)((img->pixels[(y * S_WIDTH + x) * 4] << 24) |
        ((img->pixels[(y * S_WIDTH + x) * 4 + 1]) << 16) |
        ((img->pixels[(y * S_WIDTH + x) * 4 + 2]) << 8) |
        (img->pixels[(y * S_WIDTH + x) * 4 + 3])));
}

uint32_t    choose_wall(t_game *game, int texX, int texY)
{
    uint32_t color;

    color = 0;
    if (game->r->side == 0 && game->r->rayDirX <= 0)
        color = get_color(game->sprite->north, texX, texY);
    else if (game->r->side == 0 && game->r->rayDirX >= 0)
        color = get_color(game->sprite->south, texX, texY);
    else if (game->r->side == 1 && game->r->rayDirY <= 0)
        color = get_color(game->sprite->west, texX, texY);
    else if (game->r->side == 1 && game->r->rayDirY >= 0)
        color = get_color(game->sprite->east, texX, texY);
    return (color);
}

int print_wall(t_game *game, int x, int y)
{
    uint32_t color;

    y -= 1;
    while (++y < game->r->drawEnd)
    {
        game->r->texY = (int)game->r->texPos & (S_HEIGHT - 1);
        game->r->texPos += game->r->step;
        color = choose_wall(game, game->r->texX, game->r->texY);
        if (y % DENSITY == 0)
            mlx_put_pixel(game->screen, x, y, color);
    }
    return (y);
}

void    draw_line(t_game *game, int x)
{
    int y;

    game->r->wallX = game->r->posX + game->r->perpWallDist * game->r->rayDirX;
    if (game->r->side == 0)
        game->r->wallX = game->r->posY + game->r->perpWallDist 
        * game->r->rayDirY;
    game->r->wallX -= floor(game->r->wallX);
    game->r->texX = (int)(game->r->wallX * (float)S_WIDTH);
    if ((game->r->side == 0 && game->r->rayDirX > 0) ||
    (game->r->side == 1 && game->r->rayDirY < 0))
        game->r->texX = S_WIDTH - game->r->texX - 1;
    game->r->step = 1.0f * S_HEIGHT / game->r->lineH;
    game->r->texPos = (game->r->drawStart - HEIGHT / 2 + game->r->lineH / 2)
        * game->r->step;
    y = -1;
    while (++y < game->r->drawStart)
        if (y % DENSITY == 0)
            mlx_put_pixel(game->screen, x, y , game->info->ceiling_c);
    y = print_wall(game, x, y) - 1;
    while (++y <= HEIGHT)
        if (y % DENSITY == 0)
            mlx_put_pixel(game->screen, x, y , game->info->floor_c);
}
