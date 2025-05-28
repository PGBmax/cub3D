/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:06:57 by pboucher          #+#    #+#             */
/*   Updated: 2025/05/28 14:07:53 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static mlx_image_t	*convert_and_resize(mlx_texture_t *tex, bool size)
{
	t_game		*game;
	mlx_image_t	*img;

	game = get_tgame();
	img = mlx_texture_to_image(game->mlx, tex);
	if (size)
		mlx_resize_image(img, S_WIDTH, S_HEIGHT);
	else
		mlx_resize_image(img, WIDTH, HEIGHT);
	return (img);
}

static int	load_image(t_game *game)
{
	game->textures->north = mlx_load_png(game->info->north);
	game->textures->east = mlx_load_png(game->info->east);
	game->textures->south = mlx_load_png(game->info->south);
	game->textures->west = mlx_load_png(game->info->west);
	game->textures->icon = mlx_load_png("./5_TEXTURES/0_UTILS/icon.png");
	game->textures->pause = mlx_load_png("./5_TEXTURES/0_UTILS/p_screen.png");
	if (!game->textures->east || !game->textures->west || !game->textures->north
		|| !game->textures->south || !game->textures->pause
		|| !game->textures->icon)
		return (0);
	game->sprite->north = convert_and_resize(game->textures->north, true);
	game->sprite->south = convert_and_resize(game->textures->south, true);
	game->sprite->east = convert_and_resize(game->textures->east, true);
	game->sprite->west = convert_and_resize(game->textures->west, true);
	game->sprite->pause = convert_and_resize(game->textures->pause, false);
	mlx_set_icon(game->mlx, game->textures->icon);
	return (1);
}

static void	init_player(t_game *game)
{
	game->r->dirX = -1.f;
	game->r->dirY = 0.f;
	game->r->planeX = 0.f;
	game->r->planeY = FOV;
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

int	game_init(t_game *game)
{
	game->r = ft_calloc(sizeof(t_ray), 1);
	game->textures = ft_calloc(sizeof(t_textures), 1);
	game->sprite = ft_calloc(sizeof(t_sprite), 1);
	game->r->posX = game->player->y + 0.5f;
	game->r->posY = game->player->x + 0.5f;
	game->info->ceiling_c = rgb_to_hex32(game->info->info[1]);
	game->info->floor_c = rgb_to_hex32(game->info->info[0]);
	game->paused = 0;
	if (!load_image(game))
		return (0);
	init_player(game);
	refresh(game);
	mlx_image_to_window(game->mlx, game->sprite->pause, 0, 0);
	game->sprite->pause->enabled = false;
	return (1);
}
