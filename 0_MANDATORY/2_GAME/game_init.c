/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:06:57 by pboucher          #+#    #+#             */
/*   Updated: 2025/06/17 11:40:15 by pboucher         ###   ########.fr       */
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
		mlx_resize_image(img, S_BOX, S_BOX);
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
	if (!game->textures->east || !game->textures->west
		|| !game->textures->north || !game->textures->south
		|| !game->textures->icon)
	{
		ft_printf(RED"ERROR!\n"GRN"At least one image has not loaded\n"RST);
		return (0);
	}
	game->sprite->north = convert_and_resize(game->textures->north, true);
	game->sprite->south = convert_and_resize(game->textures->south, true);
	game->sprite->east = convert_and_resize(game->textures->east, true);
	game->sprite->west = convert_and_resize(game->textures->west, true);
	mlx_set_icon(game->mlx, game->textures->icon);
	return (1);
}

static void	init_player(t_game *game)
{
	game->ray->dirX = -1.f;
	game->ray->dirY = 0.f;
	game->ray->planeX = 0.f;
	game->ray->planeY = FOV;
	if (game->info->pos == 'W')
	{
		game->ray->dirX = 0.f;
		game->ray->dirY = -1.f;
		game->ray->planeX = -FOV;
		game->ray->planeY = 0.f;
	}
	if (game->info->pos == 'S')
	{
		game->ray->dirX = 1.f;
		game->ray->dirY = 0.f;
		game->ray->planeX = 0.f;
		game->ray->planeY = -FOV;
	}
	if (game->info->pos == 'E')
	{
		game->ray->dirX = 0.f;
		game->ray->dirY = 1.f;
		game->ray->planeX = FOV;
		game->ray->planeY = 0.f;
	}
}

int	game_init(t_game *game)
{
	game->ray = ft_calloc(sizeof(t_ray), 1);
	if (!game->ray)
		return (0);
	game->textures = ft_calloc(sizeof(t_textures), 1);
	if (!game->textures)
		return (0);
	game->sprite = ft_calloc(sizeof(t_sprite), 1);
	if (!game->sprite)
	{
		free(game->textures);
		free(game->ray);
		return (0);
	}
	game->ray->posX = game->player->y + 0.5f;
	game->ray->posY = game->player->x + 0.5f;
	game->info->ceiling_c = rgb_to_hex32(game->info->info[1]);
	game->info->floor_c = rgb_to_hex32(game->info->info[0]);
	game->paused = 0;
	if (!load_image(game))
		return (0);
	init_player(game);
	refresh(game);
	return (1);
}
