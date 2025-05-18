/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:40:15 by pboucher          #+#    #+#             */
/*   Updated: 2025/05/13 16:38:03 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <stdio.h>

uint32_t rgb_to_hex32(int *rgb)
{
    return ((uint32_t)((rgb[0] << 24) | (rgb[1] << 16) | (rgb[2] << 8) | 255));
}
// Réinitialise l'image de la carte

// Dessine la carte en 2D et place le joueur si nécessaire
void draw_map(t_game *game)
{
    int y;
    int x;
    
    y = 0;
    
    // Parcourt la carte pour dessiner les éléments
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

void    draw_line(t_game *game, t_ray *r, int x)
{
    int i;
    
    i = -1;
    while (++i < r->drawStart)
        mlx_put_pixel(game->screen, x, i, rgb_to_hex32(game->info->info[1]));
    // r->drawStart -= 1;
    while (++r->drawStart <= r->drawEnd)
        mlx_put_pixel(game->screen, x, r->drawStart, r->color);
    // r->drawStart -= 1;
    while (++r->drawStart < HEIGHT)
        mlx_put_pixel(game->screen, x, r->drawStart, rgb_to_hex32(game->info->info[0]));
}

void    draw_ray(t_game *game)
{
    int x;
    
    x = -1;
    while (++x < WIDTH) 
    {
        game->r->cameraX = 2 * x / (float)WIDTH - 1;
        game->r->rayDirX = game->r->dirX + game->r->planeX * game->r->cameraX;
        game->r->rayDirY = game->r->dirY + game->r->planeY * game->r->cameraX;
        
        game->r->mapX = (int)game->r->posX;
        game->r->mapY = (int)game->r->posY;
        
        game->r->deltaDistX = (game->r->rayDirX == 0) ? 1e20 : fabs(1.0f / game->r->rayDirX);
        game->r->deltaDistY = (game->r->rayDirY == 0) ? 1e20 : fabs(1.0f / game->r->rayDirY);
        
        game->r->hit = 0;
        if (game->r->rayDirX < 0)
        {
            game->r->stepX = -1;
            game->r->sideDistX = (game->r->posX - game->r->mapX) * game->r->deltaDistX;
        }
        else
        {
            game->r->stepX = 1;
            game->r->sideDistX = (game->r->mapX + 1.0f - game->r->posX) * game->r->deltaDistX;
        }
        if (game->r->rayDirY < 0)
        {
            game->r->stepY = -1;
            game->r->sideDistY = (game->r->posY - game->r->mapY) * game->r->deltaDistY;
        }
        else
        {
            game->r->stepY = 1;
            game->r->sideDistY = (game->r->mapY + 1.0f - game->r->posY) * game->r->deltaDistY;
        }
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
            if (game->tab[game->r->mapX][game->r->mapY] == '1')
                game->r->hit = 1;
        }
        if (game->r->side == 0)
            game->r->perpWallDist = (game->r->sideDistX - game->r->deltaDistX);
        else
            game->r->perpWallDist = (game->r->sideDistY - game->r->deltaDistY);
        game->r->lineHeight = (int)(HEIGHT / game->r->perpWallDist);
        game->r->drawStart = -game->r->lineHeight / 2 + HEIGHT / 2;
        if (game->r->drawStart < 0)
            game->r->drawStart = 0;
        game->r->drawEnd = game->r->lineHeight / 2 + HEIGHT / 2;
        if (game->r->drawEnd >= HEIGHT)
            game->r->drawEnd = HEIGHT - 1;
        game->r->color = 0x00FF00FF;
        if (game->r->side == 1)
            game->r->color = game->r->color / 1.5f;
        draw_line(game, game->r, x);
    }
}

void    key_hook(t_game *game)
{
    mlx_resize_image(game->screen, 1, 1);
    mlx_put_pixel(game->screen, 0, 0, 0x00000000);
    mlx_resize_image(game->screen, WIDTH, HEIGHT);
    draw_ray(game);
    if (mlx_is_key_down(game->mlx, UP))
    {
        if (game->tab[(int)(game->r->posX + game->r->dirX * MOVESPD)][(int)game->r->posY] == '0')
            game->r->posX += game->r->dirX * MOVESPD;
        if (game->tab[(int)game->r->posX][(int)(game->r->posY + game->r->dirY * MOVESPD)] == '0')
            game->r->posY += game->r->dirY * MOVESPD;
    }
    if (mlx_is_key_down(game->mlx, DOWN))
    {
        if (game->tab[(int)(game->r->posX - game->r->dirX * MOVESPD)][(int)game->r->posY] == '0')
            game->r->posX -= game->r->dirX * MOVESPD;
        if (game->tab[(int)game->r->posX][(int)(game->r->posY - game->r->dirY * MOVESPD)] == '0')
            game->r->posY -= game->r->dirY * MOVESPD;
    }
    if (mlx_is_key_down(game->mlx, RIGHT_R))
    {
        game->r->oldDirX = game->r->dirX;
        game->r->dirX = game->r->dirX * cos(-ROTSPD) - game->r->dirY * sin(-ROTSPD);
        game->r->dirY = game->r->oldDirX * sin(-ROTSPD) + game->r->dirY * cos(-ROTSPD);
        game->r->oldPlaneX = game->r->planeX;
        game->r->planeX = game->r->planeX * cos(-ROTSPD) - game->r->planeY * sin(-ROTSPD);
        game->r->planeY = game->r->oldPlaneX * sin(-ROTSPD) + game->r->planeY * cos(-ROTSPD);
    }
    if (mlx_is_key_down(game->mlx, LEFT_R))
    {
        game->r->oldDirX = game->r->dirX;
        game->r->dirX = game->r->dirX * cos(ROTSPD) - game->r->dirY * sin(ROTSPD);
        game->r->dirY = game->r->oldDirX * sin(ROTSPD) + game->r->dirY * cos(ROTSPD);
        game->r->oldPlaneX = game->r->planeX;
        game->r->planeX = game->r->planeX * cos(ROTSPD) - game->r->planeY * sin(ROTSPD);
        game->r->planeY = game->r->oldPlaneX * sin(ROTSPD) + game->r->planeY * cos(ROTSPD);
    }
}

// Initialise le jeu et lance la boucle principale
void ft_game(t_game *game)
{
    mlx_image_t     *background;

    // Initialise la taille de la carte

    
    // Configure MLX42
    mlx_set_setting(MLX_STRETCH_IMAGE, true);
    game->mlx = mlx_init(WIDTH, HEIGHT, "cub3D", true);
    if (!game->mlx)
    error_msg(MLX_CANNOT_CREATE, NULL);
    
    // Crée un fond d'écran avec les couleurs du plafond et du sol
    background = mlx_new_image(game->mlx, 1, 1);
    // mlx_put_pixel(background, 0, 0, 0x000000FF); // Couleur du plafond
    // mlx_resize_image(background, WIDTH, HEIGHT);
    // mlx_image_to_window(game->mlx, background, 0 , 0);
    
    // Crée une image pour l'écran
    game->screen = mlx_new_image(game->mlx, WIDTH, HEIGHT);
    
    // Affiche les textures et l'écran
    mlx_image_to_window(game->mlx, game->screen, 0, 0);
    game->player = ft_calloc(sizeof(t_player), 1);
    draw_map(game);
    
    game->r = ft_calloc(sizeof(t_ray), 1);
    game->r->posX = game->player->y + 0.5f;
    game->r->posY = game->player->x + 0.5f;
    game->r->dirX = -1.f;
    game->r->dirY = 0.f;
    game->r->planeX = 0.f;
    game->r->planeY = 0.66f;
    game->r->time = game->mlx->delta_time;
    // Dessine la carte et les rayons
    
    // Configure la boucle principale
    mlx_loop_hook(game->mlx, (void (*))key_hook, (void *)game);
    mlx_loop(game->mlx);

    // Termine MLX42
    mlx_terminate(game->mlx);
}
