/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:06:57 by pboucher          #+#    #+#             */
/*   Updated: 2025/06/08 17:33:15 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"
#include <stdio.h>

static mlx_image_t	*convert_and_resize(mlx_texture_t *tex, bool size)
{
	t_game *game;
	mlx_image_t *img;

	game = get_tgame();
	img = mlx_texture_to_image(game->mlx, tex);
	if (size)
		mlx_resize_image(img, game->s_width, game->s_height);
	else
		mlx_resize_image(img, WIDTH, HEIGHT);
	return (img);
}

static uint32_t	**convert_into_matrice(mlx_image_t *img)
{
	uint32_t **matrice;
	int	i;
	int	j;

	i = 0;
	matrice = ft_calloc(sizeof(uint32_t *), S_HEIGHT + 1);
	matrice[S_HEIGHT] = NULL;
	while (i < S_HEIGHT)
	{
		matrice[i] = ft_calloc(sizeof(uint32_t), S_WIDTH + 1);
		matrice[i][S_WIDTH] = 0;
		j = 0;
		while (j < S_WIDTH)
		{
			matrice[i][j] = get_color(img, i, j);
			j++;
		}
		i++;
	}
	return (matrice);
}

static int	load_matrice(t_game *game)
{
	game->matrice->north = convert_into_matrice(game->sprite->north);
	game->matrice->south = convert_into_matrice(game->sprite->south);
	game->matrice->west = convert_into_matrice(game->sprite->west);
	game->matrice->east = convert_into_matrice(game->sprite->east);
	game->matrice->floor = convert_into_matrice(game->sprite->floor);
	game->matrice->ceilling = convert_into_matrice(game->sprite->ceilling);
	game->matrice->door = convert_into_matrice(game->sprite->door);
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
	game->textures->pause = mlx_load_png("./5_TEXTURES/0_UTILS/pause_screen.png");
	if (!game->textures->east || !game->textures->west 
		|| !game->textures->north || !game->textures->door ||
		!game->textures->south || !game->textures->pause ||
		!game->textures->floor || !game->textures->ceilling
		|| !game->textures->icon)
		return (0);
	game->sprite->north = convert_and_resize(game->textures->north, true);
	game->sprite->south = convert_and_resize(game->textures->south, true);
	game->sprite->east = convert_and_resize(game->textures->east, true);
	game->sprite->west = convert_and_resize(game->textures->west, true);
	game->sprite->pause = convert_and_resize(game->textures->pause, false);
	game->sprite->floor = convert_and_resize(game->textures->floor, true);
	game->sprite->ceilling = convert_and_resize(game->textures->ceilling, true);
	game->sprite->door = convert_and_resize(game->textures->door, true);
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

int	load_frames(t_game *game)
{
    char    *path;
    char    *modif;
    char    *modif2;
    int     i;

    i = -1;
    path = ft_strdup("./5_TEXTURES/1_FRAMES/Frame");
    while (++i < 44)
    {
        modif = ft_strdup(path);
        modif2 = ft_strjoin(modif, ft_itoa(i + 1));
        free(modif);
        modif = ft_strdup(modif2);
        free(modif2);
        modif2 = ft_strjoin(modif, ".png");
        free(modif);
        game->textures->frames[i] = mlx_load_png(modif2);
		if (!game->textures->frames[i])
		{
			free(modif2);
			return (0);
		}
        game->sprite->frames[i] = mlx_texture_to_image(game->mlx, game->textures->frames[i]);
        mlx_resize_image(game->sprite->frames[i], (int)(WIDTH / 3.368421053f), (int)(HEIGHT / 1.2f));
        mlx_image_to_window(game->mlx, game->sprite->frames[i], 0, (int)(HEIGHT / 4));
        game->sprite->frames[i]->enabled = false;
		free(modif2);
    }
	return (1);
}

int game_init(t_game *game)
{
	game->r = ft_calloc(sizeof(t_ray), 1);
	game->textures = ft_calloc(sizeof(t_textures), 1);
	game->sprite = ft_calloc(sizeof(t_sprite), 1);
	game->matrice = ft_calloc(sizeof(t_matrice), 1);
	game->r->posX = game->player->y + 0.5f;
	game->r->posY = game->player->x + 0.5f;
	game->s_height = S_HEIGHT;
	game->s_width = S_WIDTH;
	game->paused = 0;
	if (!load_image(game) || !load_frames(game) || !load_matrice(game))
		return (0);
	init_player(game);
	refresh(game);
    mlx_image_to_window(game->mlx, game->sprite->pause, 0, 0);
    game->sprite->pause->enabled = false;
	return (1);
}
