/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:06:57 by pboucher          #+#    #+#             */
/*   Updated: 2026/04/15 14:58:32 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

mlx_image_t	*convert_and_resize(mlx_texture_t *tex)
{
	t_game		*game;
	mlx_image_t	*img;

	game = get_tgame();
	img = mlx_texture_to_image(game->mlx, tex);
	mlx_resize_image(img, S_BOX, S_BOX);
	return (img);
}

uint32_t	**convert_into_matrix(mlx_image_t *img)
{
	uint32_t	**matrix;
	int			i;
	int			j;

	i = 0;
	matrix = ft_calloc(sizeof(uint32_t *), S_BOX + 1);
	if (!matrix)
		return (0);
	matrix[S_BOX] = NULL;
	while (i < S_BOX)
	{
		matrix[i] = ft_calloc(sizeof(uint32_t), S_BOX + 1);
		if (!matrix[i])
			free_matrix(matrix);
		matrix[i][S_BOX] = 0;
		j = 0;
		while (j < S_BOX)
		{
			matrix[i][j] = get_color(img, i, j);
			j++;
		}
		i++;
	}
	return (matrix);
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

static	int	init_intern_struct(t_game *game)
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
	game->matrix = ft_calloc(sizeof(t_matrix), 1);
	if (!game->matrix)
	{
		free(game->sprite);
		free(game->textures);
		free(game->ray);
		return (0);
	}
	return (1);
}

int	game_init(t_game *game)
{
	game->player = ft_calloc(sizeof(t_player), 1);
	if (!game->player)
		return (0);
	draw_map(game);
	if (!init_intern_struct(game))
		return (0);
	game->ray->posX = game->player->y + 0.5f;
	game->ray->posY = game->player->x + 0.5f;
	game->paused = 0;
	game->is_moving = 0;
	game->cur_fov = FOV;
	game->target_fov = FOV;
	if (!load_image(game) || !load_frames(game) || !load_matrix(game))
		return (0);
	mlx_set_icon(game->mlx, game->textures->icon);
	init_player(game);
	refresh(game);
	mlx_image_to_window(game->mlx, game->sprite->pause, 0, 0);
	game->sprite->pause->enabled = false;
	return (1);
}
