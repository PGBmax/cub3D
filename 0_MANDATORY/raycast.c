/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:57:43 by pboucher          #+#    #+#             */
/*   Updated: 2025/06/09 16:42:02 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void    has_touch(t_game *game)
{
    while (game->ray->hit == 0)
    {
        if (game->ray->sideDistX < game->ray->sideDistY)
        {
            game->ray->sideDistX += game->ray->deltaDistX;
            game->ray->mapX += game->ray->stepX;
            game->ray->side = 0;
        }
        else 
        {
            game->ray->sideDistY += game->ray->deltaDistY;
            game->ray->mapY += game->ray->stepY;
            game->ray->side = 1;
        }
        if (game->tab[game->ray->mapX][game->ray->mapY] == '1')
            game->ray->hit = 1;
    }
}

static void    init_ray(t_game *game, int x)
{
    game->ray->cameraX = 2 * x / (float)WIDTH - 1;
    game->ray->rayDirX = game->ray->dirX + game->ray->planeX * game->ray->cameraX;
    game->ray->rayDirY = game->ray->dirY + game->ray->planeY * game->ray->cameraX;
    game->ray->mapX = (int)game->ray->posX;
    game->ray->mapY = (int)game->ray->posY;
    if (game->ray->rayDirX == 0)
        game->ray->deltaDistX = 1e30;
    else
        game->ray->deltaDistX = fabs(1.0f / game->ray->rayDirX);
    if (game->ray->rayDirY == 0)
        game->ray->deltaDistY = 1e30;
    else
        game->ray->deltaDistY = fabs(1.0f / game->ray->rayDirY);
    game->ray->hit = 0;
}

static void    update_step(t_ray *ray)
{
    ray->stepX = 1;
    ray->sideDistX = (ray->mapX + 1.0f - ray->posX) * ray->deltaDistX;
    if (ray->rayDirX < 0)
    {
        ray->stepX = -1;
        ray->sideDistX = (ray->posX - ray->mapX) * ray->deltaDistX;
    }
    ray->stepY = 1;
    ray->sideDistY = (ray->mapY + 1.0f - ray->posY) * ray->deltaDistY;
    if (ray->rayDirY < 0)
    {
        ray->stepY = -1;
        ray->sideDistY = (ray->posY - ray->mapY) * ray->deltaDistY;
    }
}

void    draw_ray(t_game *game)
{
    int x;
    
    x = -1;
    while (++x < WIDTH) 
    {
        init_ray(game, x);
        update_step(game->ray);
        has_touch(game);
        if (game->ray->side == 0)
            game->ray->perpWallDist = (game->ray->sideDistX - game->ray->deltaDistX);
        else
            game->ray->perpWallDist = (game->ray->sideDistY - game->ray->deltaDistY);
        game->ray->lineH = (int)(HEIGHT / game->ray->perpWallDist);
        game->ray->drawStart = -game->ray->lineH / 2 + HEIGHT / 2;
        if (game->ray->drawStart < 0)
            game->ray->drawStart = 0;
        game->ray->drawEnd = game->ray->lineH / 2 + HEIGHT / 2;
        if (game->ray->drawEnd >= HEIGHT)
            game->ray->drawEnd = HEIGHT - 1;
        if (x % DENSITY == 0)
            draw_line(game, x);
    }
}
