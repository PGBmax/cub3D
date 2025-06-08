/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:57:43 by pboucher          #+#    #+#             */
/*   Updated: 2025/06/08 11:00:34 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

static void    has_touch(t_game *game)
{
    while (game->r->hit == 0)
    {
        if (game->r->sideDistX < game->r->sideDistY)
        {
            game->r->sideDistX += game->r->deltaDistX;
            game->r->mapX += game->r->stepX;
            game->r->side = 0;
        }
        else 
        {
            game->r->sideDistY += game->r->deltaDistY;
            game->r->mapY += game->r->stepY;
            game->r->side = 1;
        }
        if (game->tab[game->r->mapX][game->r->mapY] != '0')
            game->r->hit = 1;
    }
}

static void    init_ray(t_game *game, int x)
{
    game->r->cameraX = 2 * x / (float)WIDTH - 1;
    game->r->rayDirX = game->r->dirX + game->r->planeX * game->r->cameraX;
    game->r->rayDirY = game->r->dirY + game->r->planeY * game->r->cameraX;
    game->r->mapX = (int)game->r->posX;
    game->r->mapY = (int)game->r->posY;
    if (game->r->rayDirX == 0)
        game->r->deltaDistX = 1e30;
    else
        game->r->deltaDistX = fabs(1.0f / game->r->rayDirX);
    if (game->r->rayDirY == 0)
        game->r->deltaDistY = 1e30;
    else
        game->r->deltaDistY = fabs(1.0f / game->r->rayDirY);
    game->r->hit = 0;
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

void    draw_floor_ceilling(t_game *game)
{
    int x;
    int y;

    y = -1;
    while (++y < HEIGHT)
    {
        x = -1;
        float rayDirX0 = game->r->dirX - game->r->planeX;
        float rayDirY0 = game->r->dirY - game->r->planeY;
        float rayDirX1 = game->r->dirX + game->r->planeX;
        float rayDirY1 = game->r->dirY + game->r->planeY;

        int p = y - HEIGHT / 2;
        float posZ = 0.5 * HEIGHT;
        float rowDistance = posZ / p;
        float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / WIDTH;
        float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / WIDTH;
        float floorX = game->r->posX + rowDistance * rayDirX0;
        float floorY = game->r->posY + rowDistance * rayDirY0;
        while (++x < WIDTH)
        {
            int cellX = (int)floorX;
            int cellY = (int)floorY;

            int tx = (int)(game->s_width * (floorX - cellX)) & (game->s_width - 1);
            int ty = (int)(game->s_height * (floorY - cellY)) & (game->s_height - 1);
            
            floorX += floorStepX;
            floorY += floorStepY;
            uint32_t color;

            color = get_color(game->sprite->floor, tx, ty);
            mlx_put_pixel(game->screen, x, y, color);

            color = get_color(game->sprite->ceilling, tx, ty);
            mlx_put_pixel(game->screen, x, HEIGHT - y - 1, color);
        }
    }
}

void    draw_ray(t_game *game)
{
    draw_floor_ceilling(game);
    game->r->x = -1;
    while (++game->r->x < WIDTH)
    {
        init_ray(game, game->r->x);
        update_step(game->r);
        has_touch(game);
        if (game->r->side == 0)
            game->r->perpWallDist = (game->r->sideDistX - game->r->deltaDistX);
        else
            game->r->perpWallDist = (game->r->sideDistY - game->r->deltaDistY);
        game->r->lineH = (int)(HEIGHT / game->r->perpWallDist);
        game->r->drawStart = -game->r->lineH / 2 + HEIGHT / 2;
        if (game->r->drawStart < 0)
            game->r->drawStart = 0;
        game->r->drawEnd = game->r->lineH / 2 + HEIGHT / 2;
        if (game->r->drawEnd >= HEIGHT)
            game->r->drawEnd = HEIGHT - 1;
        draw_wall(game, game->r->x);
    }
}

void    detect_door(t_game *game)
{
    int x;

    x = WIDTH / 2;
    init_ray(game, x);
    update_step(game->r);
    has_touch(game);
    if (game->tab[game->r->mapX][game->r->mapY] == '2')
    {
        if ((game->r->sideDistX - game->r->deltaDistX) < 3
        && (game->r->sideDistY - game->r->deltaDistY) < 3)
            game->tab[game->r->mapX][game->r->mapY] = '3'; 
    }
    else if (game->tab[game->r->mapX][game->r->mapY] == '3')
    {
        if ((game->r->sideDistX - game->r->deltaDistX) < 3
        && (game->r->sideDistY - game->r->deltaDistY) < 3)
            game->tab[game->r->mapX][game->r->mapY] = '2';
    }
}
