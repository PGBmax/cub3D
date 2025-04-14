/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:40:15 by pboucher          #+#    #+#             */
/*   Updated: 2025/04/14 19:05:31 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <stdio.h>

void	draw_line(t_game *game, t_ray dir, int len, uint32_t color)
{
	double dx;
	double dy;
	
	dx = game->player->x;
	dy = game->player->y;
	while (len > 0) {
		printf("dx : %f\n", dx);
		printf("dy : %f\n", dy);
		mlx_put_pixel(game->screen, dx, dy, color); // function qui wrappe et protege put_pixel c'est bien
		dx += dir.DirX;
		dy += dir.DirY;
		--len;
	}
}

void	draw_ray(t_game *game)
{
	t_ray ray;

	ray.DirX = cos(game->player->a);
	ray.DirX = sin(game->player->a);
	ray.mapX = (int)(game->player->x / SIZE);
	ray.mapY = (int)(game->player->y / SIZE);
	ray.deltaDistX = fabs(1 / ray.DirX);
	ray.deltaDistY = fabs(1 / ray.DirY);
	if (ray.DirX < 0)
	{
		ray.stepX = -1;
		ray.sideDistX = (game->player->x - ray.mapX * SIZE) * ray.deltaDistX;
	}
	else 
	{
		ray.stepX = 1;
		ray.sideDistX = ((ray.mapX + 1) * SIZE - game->player->x) * ray.deltaDistX;
	}
	if (ray.DirY < 0)
	{
		ray.stepY = -1;
		ray.sideDistY = (game->player->y - ray.mapY * SIZE) * ray.deltaDistY;
	}
	else 
	{
		ray.stepY = 1;
		ray.sideDistY = ((ray.mapY + 1) * SIZE - game->player->y) * ray.deltaDistY;
	}
	while (game->tab[ray.mapY][ray.mapX] != '1')
	{
		if (ray.sideDistX < ray.sideDistY)
		{
			ray.sideDistX += ray.deltaDistX;
			ray.mapX += ray.stepX;
		}
		else
		{
			ray.sideDistY += ray.deltaDistY;
			ray.mapY += ray.stepY;
		}
	}
	ray.hitX = game->player->x + (ray.sideDistX < ray.sideDistY ? ray.sideDistX : ray.sideDistY) * ray.DirX;
	ray.hitY = game->player->y + (ray.sideDistX < ray.sideDistY ? ray.sideDistX : ray.sideDistY) * ray.DirY;
	draw_line(game, ray, 10, 0x00FF00FF);
}

void	draw_map(t_game *game)
{
	int y;
	int x;

	y = 0;
	game->sprite.wall = mlx_new_image(game->mlx, 1, 1);
	mlx_put_pixel(game->sprite.wall, 0, 0, 0xFFFFFFFF);
	mlx_resize_image(game->sprite.wall, SIZE - 2, SIZE - 2);
	game->sprite.ground = mlx_new_image(game->mlx, 1, 1);
	mlx_put_pixel(game->sprite.ground, 0, 0, 0x000000FF);
	mlx_resize_image(game->sprite.ground, SIZE - 2, SIZE - 2);
	while(game->tab[y])
	{
		x = 0;
		while (game->tab[y][x])
		{
			if (game->tab[y][x] == '1')
				mlx_image_to_window(game->mlx, game->sprite.wall, x * SIZE + 1, y * SIZE + 1);
			else if (game->tab[y][x] == '0' || game->tab[y][x] == 'N')
				mlx_image_to_window(game->mlx, game->sprite.ground, x * SIZE + 1, y * SIZE + 1);
			if (game->tab[y][x] == 'N')
			{
				game->player->x = x * SIZE + SIZE / 4;
				game->player->y = y * SIZE + SIZE / 4;
			}
			x++;
		}
		y++;
	}
	mlx_image_to_window(game->mlx, game->sprite.player, game->player->x, game->player->y);
}

void	key_hook(mlx_key_data_t key_data, t_game *game)
{
	if (key_data.action != MLX_PRESS && key_data.action != MLX_REPEAT
		&& key_data.key != MLX_KEY_ESCAPE)
		return ;
	if (key_data.key == UP1 || key_data.key == UP2)
	{
		game->player->x += game->player->dx;
		game->player->y += game->player->dy;
	}
	if (key_data.key == DOWN1 || key_data.key == DOWN2)
	{
		game->player->x -= game->player->dx;
		game->player->y -= game->player->dy;
	}
	if (key_data.key == LEFT1 || key_data.key == LEFT2)
	{
		game->player->a -= 0.1f;
		if (game->player->a < 0)
			game->player->a += 2*PI;
		game->player->dx = cos(game->player->a)*5;
		game->player->dy = sin(game->player->a)*5;
	}
	if (key_data.key == RIGHT1 || key_data.key == RIGHT2)
	{
		game->player->a += 0.1f;
		if (game->player->a > 2*PI)
			game->player->a -= 2*PI;
		game->player->dx = cos(game->player->a)*5;
		game->player->dy = sin(game->player->a)*5;
	}
	game->sprite.player->instances->x = game->player->x;
	game->sprite.player->instances->y = game->player->y;
	draw_ray(game);
	return ;
}

void	ft_game(t_game *game)
{
	game->player = ft_calloc(sizeof(t_player), 1);
	mlx_set_setting(MLX_STRETCH_IMAGE, true);
	game->mlx = mlx_init(WIDTH, HEIGHT, "cub3D", true);
	if (!game->mlx)
		error_msg(MLX_CANNOT_CREATE, NULL);
	game->sprite.player = mlx_new_image(game->mlx, 1, 2);
	game->screen = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	mlx_put_pixel(game->sprite.player, 0, 0, 0x00FF00FF);
	mlx_put_pixel(game->sprite.player, 0, 1, 0xFF0000FF);
	// game_put_pixel(game);
	mlx_resize_image(game->sprite.player, 8, 8);
	draw_map(game);
	mlx_image_to_window(game->mlx, game->screen, 0, 0);
	mlx_key_hook(game->mlx, (void (*))key_hook, (void *)game);
	mlx_loop(game->mlx);
	mlx_terminate(game->mlx);
}
