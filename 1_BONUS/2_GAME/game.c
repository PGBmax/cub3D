/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:40:15 by pboucher          #+#    #+#             */
/*   Updated: 2026/04/15 14:58:32 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"
#include <stdio.h>

void	draw_map(t_game *game)
{
	int	y;
	int	x;

	y = 0;
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

void	refresh_minimap(t_game *game)
{
	t_pos	loop;
	t_pos	center;
	t_pos	sum;
	t_color	color;

	loop = newpos(-50, -50);
	center = newpos(60, 60);
	while (loop.y < 50)
	{
		loop.x = -50;
		while (loop.x < 50)
		{
			color = pick_color(game, loop);
			sum = adding_pos(loop, center);
			draw_minimap(game, color, sum);
			loop.x++;
		}
		loop.y++;
	}
}

/*
** Met à jour le delta_time (secondes écoulées depuis la frame précédente)
** et calcule le FPS courant.
*/
static void	update_delta_time(t_game *game)
{
	double	now;

	now = mlx_get_time();
	game->delta_time = now - game->last_time;
	if (game->delta_time > 0.0)
		game->fps = (int)(1.0 / game->delta_time);
	game->last_time = now;
}

/*
** Retourne le frame d'animation courant basé sur le temps absolu
** et ANIM_FPS pour ne pas dépendre des perfs machine.
*/
static int	get_anim_frame(void)
{
	double	t;

	t = mlx_get_time();
	return ((int)(t * ANIM_FPS) % 44);
}

/*
** Dessine le compteur FPS dans un encadré en haut à droite.
** Boîte dimensionnée pour la police MLX42 (10x20 par char).
*/
static void	draw_fps_box(t_game *game, int box_w, int box_h)
{
	int	x;
	int	y;

	y = FPS_Y - FPS_PAD - 1;
	while (++y < FPS_Y + box_h + FPS_PAD)
	{
		x = FPS_X - FPS_PAD - 1;
		while (++x < FPS_X + box_w + FPS_PAD)
		{
			if (y == FPS_Y - FPS_PAD || y == FPS_Y + box_h + FPS_PAD - 1
				|| x == FPS_X - FPS_PAD || x == FPS_X + box_w + FPS_PAD - 1)
				mlx_put_pixel(game->screen, x, y, 0xFFFFFFFF);
			else
				mlx_put_pixel(game->screen, x, y, 0x000000BB);
		}
	}
}

void	update_fps(t_game *game)
{
	char	buf[16];

	snprintf(buf, sizeof(buf), "FPS: %d", game->fps);
	draw_fps_box(game, (int)ft_strlen(buf) * FPS_FONT_W, FPS_FONT_H);
	if (game->fps_img)
		mlx_delete_image(game->mlx, game->fps_img);
	game->fps_img = mlx_put_string(game->mlx, buf, FPS_X, FPS_Y);
}

void	refresh(t_game *game)
{
	int	frame;
	int	j;

	update_delta_time(game);
	frame = get_anim_frame();
	j = -1;
	while (++j < 44)
		game->sprite->frames[j]->enabled = false;
	game->sprite->frames[frame]->enabled = true;
	if (!game->paused)
		draw_ray_threaded(game);
	game->ray->moveSpeed = MOVESPD;
	game->ray->rotSpeed = ROTSPD;
	draw_vignette(game);
	draw_crosshair(game);
	refresh_minimap(game);
	update_fps(game);
}

int	ft_game(t_game *game)
{
	mlx_image_t	*background;

	mlx_set_setting(MLX_STRETCH_IMAGE, true);
	game->mlx = mlx_init(WIDTH, HEIGHT, "cub3D", true);
	if (!game->mlx)
	{
		free(game->info);
		error_msg(MLX_CANNOT_CREATE, NULL);
	}
	background = mlx_new_image(game->mlx, 1, 1);
	mlx_put_pixel(background, 0, 0, 0x000000FF);
	mlx_resize_image(background, WIDTH, HEIGHT);
	mlx_image_to_window(game->mlx, background, 0, 0);
	game->screen = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	mlx_image_to_window(game->mlx, game->screen, 0, 0);
	game->fps_img = NULL;
	game->last_time = mlx_get_time();
	if (!game_init(game))
		return (0);
	mlx_key_hook(game->mlx, (void (*))game_pause, (void *)game);
	mlx_loop_hook(game->mlx, (void (*))key_hook, (void *)game);
	mlx_cursor_hook(game->mlx, (void (*))cursor_hook, (void *)game);
	mlx_set_window_size(game->mlx, 1920, 1080);
	mlx_loop(game->mlx);
	mlx_terminate(game->mlx);
	return (1);
}

