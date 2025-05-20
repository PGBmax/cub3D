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
        if (i % DENSITY == 0)
            mlx_put_pixel(game->screen, x, i, rgb_to_hex32(game->info->info[1]));
    r->drawStart -= 1;
    while (++r->drawStart <= r->drawEnd)
        if (r->drawStart % DENSITY == 0)
            mlx_put_pixel(game->screen, x, r->drawStart, r->color);
    r->drawStart -= 1;
    while (++r->drawStart < HEIGHT)
        if (r->drawStart % DENSITY == 0)
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
        if (x % DENSITY == 0)
            draw_line(game, game->r, x);
    }
}

void    key_hook(t_game *game)
{
    mlx_resize_image(game->screen, 1, 1);
    mlx_put_pixel(game->screen, 0, 0, 0x00000000);
    mlx_resize_image(game->screen, WIDTH, HEIGHT);
    game->r->moveSpeed = MOVESPD;
    game->r->rotSpeed = ROTSPD;
    draw_ray(game);
    if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
        mlx_close_window(game->mlx);
    if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT_SHIFT) ||
        mlx_is_key_down(game->mlx, MLX_KEY_RIGHT_SHIFT))
    {
        game->r->moveSpeed *= 1.5f;
        game->r->rotSpeed *= 1.5f;
    }
    if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT_CONTROL) ||
        mlx_is_key_down(game->mlx, MLX_KEY_RIGHT_CONTROL))
    {
        game->r->moveSpeed *= 0.66f;
        game->r->rotSpeed *= 0.66f;
    }
    if (mlx_is_key_down(game->mlx, UP))
    {
        if (game->tab[(int)(game->r->posX + game->r->dirX * game->r->moveSpeed)][(int)game->r->posY] == '0')
            game->r->posX += game->r->dirX * game->r->moveSpeed;
        if (game->tab[(int)game->r->posX][(int)(game->r->posY + game->r->dirY * game->r->moveSpeed)] == '0')
            game->r->posY += game->r->dirY * game->r->moveSpeed;
    }
    if (mlx_is_key_down(game->mlx, DOWN))
    {
        if (game->tab[(int)(game->r->posX - game->r->dirX * game->r->moveSpeed)][(int)game->r->posY] == '0')
            game->r->posX -= game->r->dirX * game->r->moveSpeed;
        if (game->tab[(int)game->r->posX][(int)(game->r->posY - game->r->dirY * game->r->moveSpeed)] == '0')
            game->r->posY -= game->r->dirY * game->r->moveSpeed;
    }
    if (mlx_is_key_down(game->mlx, RIGHT))
    {
        if (game->tab[(int)(game->r->posX + game->r->planeX * game->r->moveSpeed)][(int)game->r->posY] == '0')
            game->r->posX += game->r->planeX * game->r->moveSpeed;
        if (game->tab[(int)game->r->posX][(int)(game->r->posY + game->r->planeY * game->r->moveSpeed)] == '0')
            game->r->posY += game->r->planeY * game->r->moveSpeed;
    }
    if (mlx_is_key_down(game->mlx, LEFT))
    {
        if (game->tab[(int)(game->r->posX - game->r->planeX * game->r->moveSpeed)][(int)game->r->posY] == '0')
            game->r->posX -= game->r->planeX * game->r->moveSpeed;
        if (game->tab[(int)game->r->posX][(int)(game->r->posY - game->r->planeY * game->r->moveSpeed)] == '0')
            game->r->posY -= game->r->planeY * game->r->moveSpeed;
    }
    if (mlx_is_key_down(game->mlx, RIGHT_R))
    {
        game->r->oldDirX = game->r->dirX;
        game->r->dirX = game->r->dirX * cosf(-game->r->rotSpeed) - game->r->dirY * sinf(-game->r->rotSpeed);
        game->r->dirY = game->r->oldDirX * sinf(-game->r->rotSpeed) + game->r->dirY * cosf(-game->r->rotSpeed);
        game->r->oldPlaneX = game->r->planeX;
        game->r->planeX = game->r->planeX * cosf(-game->r->rotSpeed) - game->r->planeY * sinf(-game->r->rotSpeed);
        game->r->planeY = game->r->oldPlaneX * sinf(-game->r->rotSpeed) + game->r->planeY * cosf(-game->r->rotSpeed);
    }
    if (mlx_is_key_down(game->mlx, LEFT_R))
    {
        game->r->oldDirX = game->r->dirX;
        game->r->dirX = game->r->dirX * cosf(game->r->rotSpeed) - game->r->dirY * sinf(game->r->rotSpeed);
        game->r->dirY = game->r->oldDirX * sinf(game->r->rotSpeed) + game->r->dirY * cosf(game->r->rotSpeed);
        game->r->oldPlaneX = game->r->planeX;
        game->r->planeX = game->r->planeX * cosf(game->r->rotSpeed) - game->r->planeY * sinf(game->r->rotSpeed);
        game->r->planeY = game->r->oldPlaneX * sinf(game->r->rotSpeed) + game->r->planeY * cosf(game->r->rotSpeed);
    }
}


void    cursor_hook(t_game *game)
{
    static int x = {WIDTH / 2};
    static int y = {HEIGHT / 2};
    // int oldx;
    float rot;

    rot = ROTSPD;
    mlx_set_cursor_mode(game->mlx, MLX_MOUSE_HIDDEN);
    rot = rot * (x - game->mlx->width / 2) * 0.015;
    mlx_get_mouse_pos(game->mlx, &x, &y);
    game->r->oldDirX = game->r->dirX;
    game->r->dirX = game->r->dirX * cos(-rot) - game->r->dirY * sin(-rot);
    game->r->dirY = game->r->oldDirX * sin(-rot) + game->r->dirY * cos(-rot);
    game->r->oldPlaneX = game->r->planeX;
    game->r->planeX = game->r->planeX * cos(-rot) - game->r->planeY * sin(-rot);
    game->r->planeY = game->r->oldPlaneX * sin(-rot) + game->r->planeY * cos(-rot);
    mlx_set_mouse_pos(game->mlx, game->mlx->width / 2, game->mlx->height / 2);
}

void game_init(t_game *game)
{
    game->r = ft_calloc(sizeof(t_ray), 1);
    game->r->posX = game->player->y + 0.5f;
    game->r->posY = game->player->x + 0.5f;
    // game->textures->north = mlx_load_png(game->info->north);
    // game->sprite->north = mlx_texture_to_image(game->mlx, game->textures->north);
    // mlx_resize_image(game->sprite->north, 64, 64);
    if (game->info->pos == 'N')
    {
        game->r->dirX = -1.f;
        game->r->dirY = 0.f;
        game->r->planeX = 0.f;
        game->r->planeY = FOV;
    }
    if (game->info->pos == 'W')
    {
        game->r->dirX = 0.f;
        game->r->dirY = -1.f;
        game->r->planeX = -FOV;
        game->r->planeY = 0.f;
    }
    if (game->info->pos == 'S')
    {
        game->r->dirX = 1.f;
        game->r->dirY = 0.f;
        game->r->planeX = 0.f;
        game->r->planeY = -FOV;
    }
    if (game->info->pos == 'E')
    {
        game->r->dirX = 0.f;
        game->r->dirY = 1.f;
        game->r->planeX = FOV;
        game->r->planeY = 0.f;
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
    mlx_put_pixel(background, 0, 0, 0x000000FF); // Couleur du plafond
    mlx_resize_image(background, WIDTH, HEIGHT);
    mlx_image_to_window(game->mlx, background, 0 , 0);
    
    // Crée une image pour l'écran
    game->screen = mlx_new_image(game->mlx, WIDTH, HEIGHT);
    
    // Affiche les textures et l'écran
    mlx_image_to_window(game->mlx, game->screen, 0, 0);
    game->player = ft_calloc(sizeof(t_player), 1);
    draw_map(game);
    game_init(game);
    // Dessine la carte et les rayons
    
    // Configure la boucle principale
    mlx_loop_hook(game->mlx, (void (*))key_hook, (void *)game);
    mlx_cursor_hook(game->mlx, (void (*)) cursor_hook, (void *)game);
    mlx_loop(game->mlx);

    // Termine MLX42
    mlx_terminate(game->mlx);
}
