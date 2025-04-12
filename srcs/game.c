/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:40:15 by pboucher          #+#    #+#             */
/*   Updated: 2025/04/12 19:48:51 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_map(t_game *game)
{
	int y;
	int x;

	y = 0;
	game->player->x = 0;
	game->player->y = 0;
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
				game->player->x = x * SIZE;
				game->player->y = y * SIZE;
			}
			x++;
		}
		y++;
	}
	mlx_image_to_window(game->mlx, game->sprite.fc, game->player->x, game->player->y);
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
	game->sprite.fc->instances->x = game->player->x;
	game->sprite.fc->instances->y = game->player->y;
	// draw_ray(game);
	return ;
}

void	ft_game(t_game *game)
{
	game->player = ft_calloc(sizeof(t_player), 1);
	mlx_set_setting(MLX_STRETCH_IMAGE, true);
	game->mlx = mlx_init(WIDTH, HEIGHT, "cub3D", true);
	if (!game->mlx)
		error_msg(MLX_CANNOT_CREATE, NULL);
	game->sprite.fc = mlx_new_image(game->mlx, 1, 2);
	mlx_put_pixel(game->sprite.fc, 0, 0, 0x00FF00FF);
	mlx_put_pixel(game->sprite.fc, 0, 1, 0xFF0000FF);
	// game_put_pixel(game);
	mlx_resize_image(game->sprite.fc, 10, 10);
	draw_map(game);
	mlx_key_hook(game->mlx, (void (*))key_hook, (void *)game);
	mlx_loop(game->mlx);
	mlx_terminate(game->mlx);
}
