/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:40:15 by pboucher          #+#    #+#             */
/*   Updated: 2025/04/15 18:43:23 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <stdio.h>

int touch(float px, float py, t_game *game)
{
	int x;
	int y;

	x = px / SIZE;
	y = py / SIZE;
	if (game->tab[y][x] == '1')
		return (1);
	return (0);
}

float distance(float x, float y)
{
	return (sqrt(x * x + y * y));
}

float fixed_dist(t_game *game, t_ray *ray)
{
	float delta_x;
	float delta_y;
	float angle;
	float fixed_dist;

	delta_x = ray->DirX - game->player->x;
	delta_y = ray->DirY - game->player->y;
	angle = atan2(delta_y, delta_x) - game->player->a;
	fixed_dist = distance(delta_x, delta_y) * cos(angle);
	return (fixed_dist);
	
}

void	draw_line(t_game *game, float start_x, int i)
{
	t_ray ray;
	float cos_angle;
	float sin_angle;
	float dist;
	float height;
	float start_y;
	float end;

	ray.DirX = game->player->x;
	ray.DirY = game->player->y;
	cos_angle = cos(start_x);
	sin_angle = sin(start_x);
	while (!touch(ray.DirX, ray.DirY, game))
	{
		if (DEBUG)
			mlx_put_pixel(game->screen, ray.DirX, ray.DirY, 0x0000FFFF);
		ray.DirX += cos_angle;
		ray.DirY += sin_angle;
	}
	if (!DEBUG)
	{
		// dist = distance(ray.DirX - game->player->x, ray.DirY - game->player->y);
		dist = fixed_dist(game, &ray);
		height = (SIZE / dist) * (WIDTH / 2.f);
		start_y = (HEIGHT - height) / 2.f;
		end = start_y + height;
		while (start_y < end)
		{
			if (start_y >= 0)
				mlx_put_pixel(game->screen, i, start_y, 0x999999FF);
			start_y++;
		}
	}
}

void	draw_ray(t_game *game)
{
	int i;
	float frac;
	float start_x;

	i = 0;
	frac = PI / 3.f / WIDTH;
	start_x = game->player->a - PI / 6.f;
	while (i < WIDTH)
	{
		draw_line(game, start_x, i);
		start_x += frac;
		i++;
	}
}

void	draw_square(t_game *game, int x, int y, uint32_t color)
{
	int i;
	int j;

	i = 0;
	while (i < SIZE)
	{
		j = 0;
		while (j < SIZE)
		{
			mlx_put_pixel(game->screen, x + j, y + i, color);
			if (DEBUG)
			{
				if (i == 0 || i == SIZE - 1 || j == 0 || j == SIZE - 1)
					mlx_put_pixel(game->screen, x + j, y + i, 0x444444FF);
				else
					mlx_put_pixel(game->screen, x + j, y + i, color);
			}
			j++;
		}
		i++;
	}
}

void	draw_map(t_game *game, int check)
{
	int y;
	int x;

	y = 0;
	while(game->tab[y])
	{
		x = 0;
		while (game->tab[y][x])
		{
			draw_square(game, x * SIZE, y * SIZE, 0x00000000);
			if (DEBUG)
			{
				if (game->tab[y][x] == '1')
					draw_square(game, x * SIZE, y * SIZE, 0xFFFFFFFF);
				else if (game->tab[y][x] == '0' || game->tab[y][x] == 'N')
					draw_square(game, x * SIZE, y * SIZE, 0x000000FF);
			}
			if (game->tab[y][x] == 'N' && check == 1)
			{
				game->player->x = x * SIZE + SIZE / 2.f;
				game->player->y = y * SIZE + SIZE / 2.f;
			}
			x++;
		}
		y++;
	}
}

void	key_hook(t_game *game)
{
	if (mlx_is_key_down(game->mlx, UP1) || mlx_is_key_down(game->mlx, UP2))
	{
		game->player->x += game->player->dx;
		game->player->y += game->player->dy;
	}
	if (mlx_is_key_down(game->mlx, DOWN1) || mlx_is_key_down(game->mlx, DOWN2))
	{
		game->player->x -= game->player->dx;
		game->player->y -= game->player->dy;
	}
	if (mlx_is_key_down(game->mlx, LEFT1))
	{
		game->player->x += game->player->dy;
		game->player->y -= game->player->dx;
	}
	if (mlx_is_key_down(game->mlx, RIGHT1))
	{
		game->player->x -= game->player->dy;
		game->player->y += game->player->dx;
	}
	if (mlx_is_key_down(game->mlx, LEFT2))
	{
		game->player->a -= RADIANS;
		if (game->player->a < 0)
			game->player->a += 2.f * PI;
		game->player->dx = cos(game->player->a)*4.f;
		game->player->dy = sin(game->player->a)*4.f;
	}
	if (mlx_is_key_down(game->mlx, RIGHT2))
	{
		game->player->a += RADIANS;
		if (game->player->a > 2.f * PI)
			game->player->a -= 2.f * PI;
		game->player->dx = cos(game->player->a)*4.f;
		game->player->dy = sin(game->player->a)*4.f;
	}
	if (DEBUG)
	{
		game->sprite.player->instances->x = game->player->x;
		game->sprite.player->instances->y = game->player->y;
	}
	draw_map(game, 0);
	draw_ray(game);
}

void	ft_game(t_game *game)
{
	mlx_image_t *background;
	game->player = ft_calloc(sizeof(t_player), 1);
	game->player->dx = 1;
	game->player->dy = 1;
	mlx_set_setting(MLX_STRETCH_IMAGE, true);
	game->mlx = mlx_init(WIDTH, HEIGHT, "cub3D", true);
	if (!game->mlx)
		error_msg(MLX_CANNOT_CREATE, NULL);
	if (DEBUG)
	{
		game->sprite.player = mlx_new_image(game->mlx, 1, 1);
		mlx_put_pixel(game->sprite.player, 0, 0, 0x00FF00FF);
	}
	background = mlx_new_image(game->mlx, 1, 2);
	mlx_put_pixel(background, 0, 0, 0xFFFFFFFF);
	mlx_put_pixel(background, 0, 1, 0x000000FF);
	mlx_resize_image(background, WIDTH, HEIGHT);
	mlx_image_to_window(game->mlx, background, 0 , 0);
	game->screen = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	mlx_image_to_window(game->mlx, game->screen, 0, 0);
	draw_map(game, 1);
	draw_ray(game);
	if (DEBUG)
	{
		mlx_resize_image(game->sprite.player, 8, 8);
		mlx_image_to_window(game->mlx, game->sprite.player, game->player->x, game->player->y);
	}
	mlx_loop_hook(game->mlx, (void (*))key_hook, (void *)game);
	mlx_loop(game->mlx);
	mlx_terminate(game->mlx);
}
