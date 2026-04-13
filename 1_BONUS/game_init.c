/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:06:57 by pboucher          #+#    #+#             */
/*   Updated: 2025/08/16 04:54:36 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"
#include <stdio.h>

static mlx_image_t	*convert_and_resize(mlx_texture_t *tex)
{
	t_game *game;
	mlx_image_t *img;

	game = get_tgame();
	img = mlx_texture_to_image(game->mlx, tex);
	mlx_resize_image(img, S_BOX, S_BOX);
	return (img);
}

static uint32_t	**convert_into_matrix(mlx_image_t *img)
{
	uint32_t **matrix;
	int	i;
	int	j;

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

static int	load_matrix(t_game *game)
{
	game->matrix->north = convert_into_matrix(game->sprite->north);
	game->matrix->south = convert_into_matrix(game->sprite->south);
	game->matrix->west = convert_into_matrix(game->sprite->west);
	game->matrix->east = convert_into_matrix(game->sprite->east);
	game->matrix->floor = convert_into_matrix(game->sprite->floor);
	game->matrix->ceilling = convert_into_matrix(game->sprite->ceilling);
	game->matrix->door = convert_into_matrix(game->sprite->door);
	return (1);
}

static int	load_image(t_game *game)
{
	game->textures->north = mlx_load_png(game->info->north);
	game->textures->east = mlx_load_png(game->info->east);
	game->textures->south = mlx_load_png(game->info->south);
	game->textures->west = mlx_load_png(game->info->west);
	game->textures->floor = mlx_load_png(game->info->floor);
	game->textures->ceilling = mlx_load_png(game->info->ceiling);
	game->textures->door = mlx_load_png(game->info->door);
	game->textures->icon = mlx_load_png("./5_TEXTURES/0_UTILS/icon.png");
	if (!game->textures->east || !game->textures->west 
		|| !game->textures->north || !game->textures->door ||
		!game->textures->south || 
		!game->textures->floor || !game->textures->ceilling
		|| !game->textures->icon)
		return (0);
	game->sprite->north = convert_and_resize(game->textures->north);
	game->sprite->south = convert_and_resize(game->textures->south);
	game->sprite->east = convert_and_resize(game->textures->east);
	game->sprite->west = convert_and_resize(game->textures->west);
	game->sprite->pause = mlx_new_image(game->mlx, 1, 1);
	mlx_put_pixel(game->sprite->pause, 0, 0, 0x000000FF - 128);
	mlx_resize_image(game->sprite->pause, WIDTH, HEIGHT);
	game->sprite->floor = convert_and_resize(game->textures->floor);
	game->sprite->ceilling = convert_and_resize(game->textures->ceilling);
	game->sprite->door = convert_and_resize(game->textures->door);
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

int	load_frames(t_game *game)
{
    char    *path;
    char    *modif;
    char    *modif2;
	char	*num;
    int     i;

    i = -1;
    path = ft_strdup("./5_TEXTURES/1_FRAMES/Frame");
    while (++i < 44)
    {
        modif = ft_strdup(path);
		num = ft_itoa(i + 1);
        modif2 = ft_strjoin(modif, num);
		free(num);
        free(modif);
        modif = ft_strdup(modif2);
        free(modif2);
        modif2 = ft_strjoin(modif, ".png");
        free(modif);
        game->textures->frames[i] = mlx_load_png(modif2);
		if (!game->textures->frames[i])
		{
			free(modif2);
			free(path);
			return (0);
		}
        game->sprite->frames[i] = mlx_texture_to_image(game->mlx, game->textures->frames[i]);
        mlx_resize_image(game->sprite->frames[i], (int)(WIDTH / 3.368421053f), (int)(HEIGHT / 1.2f));
        mlx_image_to_window(game->mlx, game->sprite->frames[i], 0, (int)(HEIGHT / 4));
        game->sprite->frames[i]->enabled = false;
		free(modif2);
    }
	i = -1;
    path = ft_strdup("./5_TEXTURES/1_FRAMES/door");
    while (++i < 30)
    {
        modif = ft_strdup(path);
		num = ft_itoa(i + 1);
        modif2 = ft_strjoin(modif, num);
		free(num);
        free(modif);
        modif = ft_strdup(modif2);
        free(modif2);
        modif2 = ft_strjoin(modif, ".png");
        free(modif);
        game->textures->door2[i] = mlx_load_png(modif2);
		if (!game->textures->door2[i])
		{
			free(modif2);
			free(path);
			return (0);
		}
        game->sprite->door2[i] = mlx_texture_to_image(game->mlx, game->textures->door2[i]);
        mlx_resize_image(game->sprite->door2[i], S_BOX, S_BOX);
		game->matrix->door2[i] = convert_into_matrix(game->sprite->door2[i]);
		free(modif2);
    }
	free(path);
	return (1);
}

int game_init(t_game *game)
{
	game->player = ft_calloc(sizeof(t_player), 1);
	if (!game->player)
		return (0);
	draw_map(game);
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
	game->ray->posX = game->player->y + 0.5f;
	game->ray->posY = game->player->x + 0.5f;
	game->paused = 0;
	if (!load_image(game) || !load_frames(game) || !load_matrix(game))
		return (0);
	init_player(game);
	refresh(game);
    mlx_image_to_window(game->mlx, game->sprite->pause, 0, 0);
    game->sprite->pause->enabled = false;
	return (1);
}
