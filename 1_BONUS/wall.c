/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 15:14:23 by pboucher          #+#    #+#             */
/*   Updated: 2025/06/10 13:49:39 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

uint32_t get_color(mlx_image_t *img, int x, int y)
{
    t_game  *game;

    game = get_tgame();
    return ((uint32_t)((img->pixels[(y * S_BOX + x) * 4] << 24) |
        ((img->pixels[(y * S_BOX + x) * 4 + 1]) << 16) |
        ((img->pixels[(y * S_BOX + x) * 4 + 2]) << 8) |
        (img->pixels[(y * S_BOX + x) * 4 + 3])));
}



uint32_t    choose_wall(t_game *game, int texX, int texY)
{
    uint32_t color;

    color = 0;
    if (game->tab[game->ray->mapX][game->ray->mapY] == '2')
    {
        color = game->matrix->door[texX][texY];
        return (color);
    }
    if (game->tab[game->ray->mapX][game->ray->mapY] == '3')
    {
        color = game->matrix->door[texX][texY] - 128;
        return (color);
    }
    if (game->ray->side == 0 && game->ray->rayDirX <= 0)
        color = game->matrix->north[texX][texY];
    else if (game->ray->side == 0 && game->ray->rayDirX >= 0)
        color = game->matrix->south[texX][texY];
    else if (game->ray->side == 1 && game->ray->rayDirY <= 0)
        color = game->matrix->west[texX][texY];
    else if (game->ray->side == 1 && game->ray->rayDirY >= 0)
        color = game->matrix->east[texX][texY];
    return (color);
}

void    print_wall(t_game *game, int x)
{
    uint32_t color;

    game->ray->drawStart -= 1;
    while (++game->ray->drawStart <= game->ray->drawEnd)
    {
        game->ray->texY = (int)game->ray->texPos & (S_BOX - 1);
        game->ray->texPos += game->ray->step;
        color = choose_wall(game, game->ray->texX, game->ray->texY);
        mlx_put_pixel(game->screen, x, game->ray->drawStart, color);
    }
}

void    draw_wall(t_game *game, int x)
{
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
    game->ray->texPos = (game->ray->drawStart - HEIGHT / 2 + game->ray->lineH / 2) * game->ray->step;
    print_wall(game, x);
}
