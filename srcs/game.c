/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maregnie <maregnie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:40:15 by pboucher          #+#    #+#             */
/*   Updated: 2025/04/18 16:57:28 by maregnie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <stdio.h>

int touch(float px, float py, t_game *game)
{
	int x1;
	int y1;
	int x2;
	int y2;

	x1 = px / SIZE;
	y1 = py / SIZE;
	x2 = game->player->x / SIZE;
	y2 = game->player->y / SIZE;
	if (y1 < 0 || y1 >= game->mapsize[1] || x1 < 0 || x1 >= game->mapsize[0] || game->tab[y1][x1] != game->tab[y2][x2])
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
	fixed_dist = distance(delta_x, delta_y) * cos(angle) * 0.55f;
	printf("%f\n", fixed_dist);
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
		ray.DirX += cos_angle;
		ray.DirY += sin_angle;
	}
	dist = fixed_dist(game, &ray);
	height = (SIZE / dist) * (WIDTH / 2.f);
	start_y = (HEIGHT - height) / 2.f;
	end = start_y + height;
	while (start_y < end && start_y < HEIGHT)
	{
		if (start_y >= 0)
			mlx_put_pixel(game->screen, i, start_y, 0x111111FF);
		start_y++;
	}
}

void	draw_ray(t_game *game)
{
	int i;
	float frac;
	float start_x;

	i = 0;
	frac = PI / 3.f / WIDTH;
	start_x = game->player->a - PI / (2.f*PI);
	while (i < WIDTH)
	{
		draw_line(game, start_x, i);
		start_x += frac;
		i++;
	}
}

void	draw_map(t_game *game, int check)
{
	int y;
	int x;

	y = 0;
	mlx_resize_image(game->screen, 1, 1);
	mlx_put_pixel(game->screen, 0, 0, 0x00000000);
	mlx_resize_image(game->screen, WIDTH, HEIGHT);
	while(game->tab[y])
	{
		x = 0;
		while (game->tab[y][x + 1])
		{
			if (game->tab[y][x] == 'N' && check == 1)
			{
				game->player->x = x * SIZE + SIZE / 2.f;
				game->player->y = y * SIZE + SIZE / 2.f;
				game->tab[y][x] = '0';
			}
			x++;
		}
		y++;
	}
}

void	make_move(t_game *game, double dx, double dy)
{
	if ((mlx_is_key_down(game->mlx, MLX_KEY_LEFT_SHIFT)
		|| mlx_is_key_down(game->mlx, MLX_KEY_RIGHT_SHIFT))
		&& (!mlx_is_key_down(game->mlx, MLX_KEY_LEFT_CONTROL)
		&& (!mlx_is_key_down(game->mlx, MLX_KEY_RIGHT_CONTROL))))
	{
		dx *= 1.5f;
		dy *= 1.5f;
	}
	if ((mlx_is_key_down(game->mlx, MLX_KEY_LEFT_CONTROL)
		|| mlx_is_key_down(game->mlx, MLX_KEY_RIGHT_CONTROL))
		&& (!mlx_is_key_down(game->mlx, MLX_KEY_LEFT_SHIFT)
		&& (!mlx_is_key_down(game->mlx, MLX_KEY_RIGHT_SHIFT))))
	{
		dx *= 0.5f;
		dy *= 0.5f;
	}
	game->player->x += dx;
	game->player->y += dy;
}

void	key_hook(t_game *game)
{
	if (mlx_is_key_down(game->mlx, UP1) || mlx_is_key_down(game->mlx, UP2))
		make_move(game, game->player->dx, game->player->dy);
	if (mlx_is_key_down(game->mlx, DOWN1) || mlx_is_key_down(game->mlx, DOWN2))
		make_move(game, -1.f * game->player->dx, -1.f * game->player->dy);
	if (mlx_is_key_down(game->mlx, LEFT1))
		make_move(game, game->player->dy, -1.f * game->player->dx);
	if (mlx_is_key_down(game->mlx, RIGHT1))
		make_move(game, -1.f * game->player->dy, game->player->dx);
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
		game->sprite->player->instances->x = game->player->x;
		game->sprite->player->instances->y = game->player->y;
	}
	draw_map(game, 0);
	draw_ray(game);
}

uint32_t rgb_to_hex32(int *rgb)
{
    return ((uint32_t)rgb[0] << 24) | ((uint32_t)rgb[1] << 16) | ((uint32_t)rgb[2] << 8) | 255;
}

void	ft_game(t_game *game)
{
	mlx_image_t *background;

	game->mapsize[1] = ft_tablen(game->tab);
	game->mapsize[0] = ft_strlen(game->tab[0]);
	game->player = ft_calloc(sizeof(t_player), 1);
	game->player->a = 300;
	game->player->dx = cos(game->player->a)*4.f;
	game->player->dy = sin(game->player->a)*4.f;
	mlx_set_setting(MLX_STRETCH_IMAGE, true);
	game->mlx = mlx_init(WIDTH, HEIGHT, "cub3D", true);
	if (!game->mlx)
		error_msg(MLX_CANNOT_CREATE, NULL);
	background = mlx_new_image(game->mlx, 1, 2);
	mlx_put_pixel(background, 0, 0, rgb_to_hex32(game->info->info[1]));
	mlx_put_pixel(background, 0, 1, rgb_to_hex32(game->info->info[0]));
	mlx_resize_image(background, WIDTH, HEIGHT);
	mlx_image_to_window(game->mlx, background, 0 , 0);
	game->screen = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	convert_textures(game);
	mlx_image_to_window(game->mlx, game->sprite->wall, (int)roundf(game->player->x), (int)roundf(game->player->y));
	mlx_image_to_window(game->mlx, game->screen, 0, 0);
	draw_map(game, 1);
	draw_ray(game);
	mlx_loop_hook(game->mlx, (void (*))key_hook, (void *)game);
	mlx_loop(game->mlx);
	mlx_terminate(game->mlx);
} 