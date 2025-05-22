/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:40:15 by pboucher          #+#    #+#             */
/*   Updated: 2025/05/22 20:20:32 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <stdio.h>

uint32_t    rgb_to_hex32(int *rgb)
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

uint32_t get_color(mlx_image_t *img, int x, int y)
{
    return ((uint32_t)((img->pixels[(y * S_WIDTH + x) * 4] << 24) |
        ((img->pixels[(y * S_WIDTH + x) * 4 + 1]) << 16) |
        ((img->pixels[(y * S_WIDTH + x) * 4 + 2]) << 8) |
        (img->pixels[(y * S_WIDTH + x) * 4 + 3])));
}

uint32_t    print_wall(t_game *game, int texX, int texY)
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

void    draw_line(t_game *game, int x)
{
    float wallX;

    if (game->r->side == 0)
        wallX = game->r->posY + game->r->perpWallDist * game->r->rayDirY;
    else
        wallX = game->r->posX + game->r->perpWallDist * game->r->rayDirX;
    wallX -= floor(wallX);
    int texX = (int)(wallX * (float)S_WIDTH);
    if ((game->r->side == 0 && game->r->rayDirX > 0) ||
    (game->r->side == 1 && game->r->rayDirY < 0))
        texX = S_WIDTH - texX - 1;
    float step = 1.0f * S_HEIGHT / game->r->lineHeight;
    float texPos = (game->r->drawStart - HEIGHT / 2 + game->r->lineHeight / 2) * step;
    int y;
    y = -1;
    while (++y < game->r->drawStart)
        if (y % DENSITY == 0)
            mlx_put_pixel(game->screen, x, y , game.);
    y -= 1;
    step = 1.0f * S_HEIGHT / game->r->lineHeight;
    texPos = (game->r->drawStart - HEIGHT / 2 + game->r->lineHeight / 2) * step;
    while (++y < game->r->drawEnd)
    {
        int texY = (int)texPos & (S_HEIGHT - 1);
        texPos += step;
        if (y % DENSITY == 0)
            mlx_put_pixel(game->screen, x, y, print_wall(game, texX, texY));
    }
    step = 1.0f * S_HEIGHT / game->r->lineHeight;
    texPos = (game->r->drawStart - HEIGHT / 2 + game->r->lineHeight / 2) * step;
    y -= 1; 
    while (++y < HEIGHT)
    {
        int texY = (int)texPos & (S_HEIGHT - 1);
        texPos += step;
        if (y % DENSITY == 0)
            mlx_put_pixel(game->screen, x, y , (print_wall(game, texX, texY) >> 1) & 8355711);
    }
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
        
        game->r->deltaDistX = (game->r->rayDirX == 0) ? 1e30 : fabs(1.0f / game->r->rayDirX);
        game->r->deltaDistY = (game->r->rayDirY == 0) ? 1e30 : fabs(1.0f / game->r->rayDirY);
        
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
        if (x % DENSITY == 0)
            draw_line(game, x);
    }
}

void refresh(t_game *game)
{
    draw_ray(game);
    game->r->moveSpeed = MOVESPD;
    game->r->rotSpeed = ROTSPD;
}

void    change_speed(t_game *game, float move, float rotate)
{
    game->r->moveSpeed *= move;
    game->r->rotSpeed *= rotate;
}

void    move_player(t_game *game, float compX, float compY)
{
    if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT_SHIFT) ||
        mlx_is_key_down(game->mlx, MLX_KEY_RIGHT_SHIFT))
        change_speed(game, 1.5f, 1.5f);
    if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT_CONTROL) ||
        mlx_is_key_down(game->mlx, MLX_KEY_RIGHT_CONTROL))
        change_speed(game, 0.66f, 0.66f);
    if (!(int)(game->r->posX + compX * game->r->moveSpeed) && !game->tab[(int)game->r->posX][(int)(game->r->posY + compY * game->r->moveSpeed)])
        return;
    if (game->tab[(int)(game->r->posX + compX * game->r->moveSpeed)][(int)game->r->posY] == '0')
        game->r->posX += compX * game->r->moveSpeed;
    if (game->tab[(int)game->r->posX][(int)(game->r->posY + compY * game->r->moveSpeed)] == '0')
        game->r->posY += compY * game->r->moveSpeed;
}

void    rotate_cam(t_game *game, float rotSpeed)
{

    if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT_SHIFT) ||
        mlx_is_key_down(game->mlx, MLX_KEY_RIGHT_SHIFT))
        change_speed(game, 1.5f, 1.5f);
    if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT_CONTROL) ||
        mlx_is_key_down(game->mlx, MLX_KEY_RIGHT_CONTROL))
        change_speed(game, 0.66f, 0.66f);
    game->r->oldDirX = game->r->dirX;
    game->r->dirX = game->r->dirX * cosf(rotSpeed) - game->r->dirY * sinf(rotSpeed);
    game->r->dirY = game->r->oldDirX * sinf(rotSpeed) + game->r->dirY * cosf(rotSpeed);
    game->r->oldPlaneX = game->r->planeX;
    game->r->planeX = game->r->planeX * cosf(rotSpeed) - game->r->planeY * sinf(rotSpeed);
    game->r->planeY = game->r->oldPlaneX * sinf(rotSpeed) + game->r->planeY * cosf(rotSpeed);
}


bool    need_refresh(t_game *game)
{
    if (mlx_is_key_down(game->mlx, UP) ||
        mlx_is_key_down(game->mlx, DOWN) || 
        mlx_is_key_down(game->mlx, RIGHT) || 
        mlx_is_key_down(game->mlx, LEFT) || 
        mlx_is_key_down(game->mlx, RIGHT_R) || 
        mlx_is_key_down(game->mlx, LEFT_R))
        return (true);
    return (false);
}

void    key_hook(t_game *game)
{
    if (!game->paused)
		refresh(game);
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
}

void    cursor_hook(t_game *game)
{
    static int x = {WIDTH / 2};
    static int y = {HEIGHT / 2};
    float rot;

	if (!game->paused)
	{
		rot = ROTSPD;
		mlx_set_cursor_mode(game->mlx, MLX_MOUSE_HIDDEN);
		rot = rot * (x - game->mlx->width / 2) * 0.025;
		mlx_get_mouse_pos(game->mlx, &x, &y);
		rotate_cam(game, -rot);
		mlx_set_mouse_pos(game->mlx, game->mlx->width / 2, game->mlx->height / 2);
	}
    if (game->paused)
		mlx_set_cursor_mode(game->mlx, MLX_MOUSE_NORMAL);
}

int game_init(t_game *game)
{
    game->r = ft_calloc(sizeof(t_ray), 1);
    game->textures = ft_calloc(sizeof(t_textures), 1);
    game->sprite = ft_calloc(sizeof(t_sprite), 1);
    game->r->posX = game->player->y + 0.5f;
    game->r->posY = game->player->x + 0.5f;
    game->textures->north = mlx_load_png(game->info->north);
    game->textures->east = mlx_load_png(game->info->east);
    game->textures->south = mlx_load_png(game->info->south);
    game->textures->west = mlx_load_png(game->info->west);
    game->textures->icon = mlx_load_png("./textures/icon.png");
    game->textures->pause = mlx_load_png("./textures/utils/pause_screen.png");
    if (!game->textures->east || !game->textures->west || !game->textures->north ||
        !game->textures->south || !game->textures->pause || !game->textures->icon)
        return (0);
    game->sprite->north = mlx_texture_to_image(game->mlx, game->textures->north);
    game->sprite->south = mlx_texture_to_image(game->mlx, game->textures->south);
    game->sprite->west = mlx_texture_to_image(game->mlx, game->textures->west);
    game->sprite->east = mlx_texture_to_image(game->mlx, game->textures->east);
	game->paused = 0;
    mlx_resize_image(game->sprite->north, S_WIDTH, S_HEIGHT);
    mlx_resize_image(game->sprite->south, S_WIDTH, S_HEIGHT);
    mlx_resize_image(game->sprite->west, S_WIDTH, S_HEIGHT);
    mlx_resize_image(game->sprite->east, S_WIDTH, S_HEIGHT);
    game->sprite->pause = mlx_texture_to_image(game->mlx, game->textures->pause);
    game->info->ceiling_c = rgb_to_hex32(game->info->info[1]);
    game->info->floor_c = rgb_to_hex32(game->info->info[0]);
    mlx_resize_image(game->sprite->pause, WIDTH, HEIGHT);
    mlx_set_icon(game->mlx, game->textures->icon);
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
    return (1);
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

// Initialise le jeu et lance la boucle principale
int ft_game(t_game *game)
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
    if (!game_init(game))
        return (0);
    refresh(game);
    mlx_image_to_window(game->mlx, game->sprite->pause, 0, 0);
    game->sprite->pause->enabled = false;
    
    // Dessine la carte et les rayons
    
    // Configure la boucle principale
    mlx_key_hook(game->mlx, (void (*))game_pause, (void *)game);
    mlx_loop_hook(game->mlx, (void (*))key_hook, (void *)game);
    mlx_cursor_hook(game->mlx, (void (*)) cursor_hook, (void *)game);
    mlx_loop(game->mlx);

    // Termine MLX42
    mlx_terminate(game->mlx);
    return (1);
}
