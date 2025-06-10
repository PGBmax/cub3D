/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 15:14:23 by pboucher          #+#    #+#             */
/*   Updated: 2025/06/09 16:42:02 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

uint32_t get_color(mlx_image_t *img, int x, int y)
{
    return ((uint32_t)((img->pixels[(y * S_BOX + x) * 4] << 24) |
        ((img->pixels[(y * S_BOX + x) * 4 + 1]) << 16) |
        ((img->pixels[(y * S_BOX + x) * 4 + 2]) << 8) |
        (img->pixels[(y * S_BOX + x) * 4 + 3])));
}

uint32_t    choose_wall(t_game *game, int texX, int texY)
{
    uint32_t color;

    color = 0;
    if (game->ray->side == 0 && game->ray->rayDirX <= 0)
        color = get_color(game->sprite->north, texX, texY);
    else if (game->ray->side == 0 && game->ray->rayDirX >= 0)
        color = get_color(game->sprite->south, texX, texY);
    else if (game->ray->side == 1 && game->ray->rayDirY <= 0)
        color = get_color(game->sprite->west, texX, texY);
    else if (game->ray->side == 1 && game->ray->rayDirY >= 0)
        color = get_color(game->sprite->east, texX, texY);
    return (color);
}

int print_wall(t_game *game, int x, int y)
{
    uint32_t color;

    y -= 1;
    while (++y < game->ray->drawEnd)
    {
        game->ray->texY = (int)game->ray->texPos & (S_BOX - 1);
        game->ray->texPos += game->ray->step;
        color = choose_wall(game, game->ray->texX, game->ray->texY);
        if (y % DENSITY == 0)
            mlx_put_pixel(game->screen, x, y, color);
    }
    return (y);
}

void    draw_line(t_game *game, int x)
{
    int y;

    game->ray->wallX = game->ray->posX + game->ray->perpWallDist * game->ray->rayDirX;
    if (game->ray->side == 0)
        game->ray->wallX = game->ray->posY + game->ray->perpWallDist 
        * game->ray->rayDirY;
    game->ray->wallX -= floor(game->ray->wallX);
    game->ray->texX = (int)(game->ray->wallX * (float)S_BOX);
    if ((game->ray->side == 0 && game->ray->rayDirX > 0) ||
    (game->ray->side == 1 && game->ray->rayDirY < 0))
        game->ray->texX = S_BOX - game->ray->texX - 1;
    game->ray->step = 1.0f * S_BOX / game->ray->lineH;
    game->ray->texPos = (game->ray->drawStart - HEIGHT / 2 + game->ray->lineH / 2)
        * game->ray->step;
    y = -1;
    while (++y < game->ray->drawStart)
        if (y % DENSITY == 0)
            mlx_put_pixel(game->screen, x, y , game->info->ceiling_c);
    y = print_wall(game, x, y) - 1;
    while (++y < HEIGHT)
        if (y % DENSITY == 0)
            mlx_put_pixel(game->screen, x, y , game->info->floor_c);
}
