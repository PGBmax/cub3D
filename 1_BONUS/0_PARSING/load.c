/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 15:57:59 by pboucher          #+#    #+#             */
/*   Updated: 2025/06/17 11:30:57 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

int	assign_frame(t_game *game, char **modif2, char **path, int i)
{
	game->textures->frames[i] = mlx_load_png(*modif2);
	if (!game->textures->frames[i])
	{
		free(*modif2);
		free(*path);
		return (0);
	}
	game->sprite->frames[i] = mlx_texture_to_image(game->mlx,
			game->textures->frames[i]);
	mlx_resize_image(game->sprite->frames[i],
		(int)(WIDTH / 3.368421053f), (int)(HEIGHT / 1.2f));
	mlx_image_to_window(game->mlx, game->sprite->frames[i], 0,
		(int)(HEIGHT / 4));
	game->sprite->frames[i]->enabled = false;
	free(*modif2);
	return (1);
}

int	load_frames(t_game *game)
{
	char	*path;
	char	*modif;
	char	*modif2;
	char	*num;
	int		i;

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
		if (!assign_frame(game, &modif2, &path, i))
			return (0);
	}
	free(path);
	return (1);
}

void	convert_image(t_game *game)
{
	game->sprite->north = convert_and_resize(game->textures->north);
	game->sprite->south = convert_and_resize(game->textures->south);
	game->sprite->east = convert_and_resize(game->textures->east);
	game->sprite->west = convert_and_resize(game->textures->west);
	game->sprite->pause = mlx_texture_to_image(game->mlx,
			game->textures->pause);
	mlx_resize_image(game->sprite->pause, WIDTH, HEIGHT);
	game->sprite->floor = convert_and_resize(game->textures->floor);
	game->sprite->ceilling = convert_and_resize(game->textures->ceilling);
	game->sprite->door = convert_and_resize(game->textures->door);
}

int	load_image(t_game *game)
{
	game->textures->north = mlx_load_png(game->info->north);
	game->textures->east = mlx_load_png(game->info->east);
	game->textures->south = mlx_load_png(game->info->south);
	game->textures->west = mlx_load_png(game->info->west);
	game->textures->floor = mlx_load_png(game->info->floor);
	game->textures->ceilling = mlx_load_png(game->info->ceiling);
	game->textures->door = mlx_load_png(game->info->door);
	game->textures->icon = mlx_load_png("./5_TEXTURES/0_UTILS/icon.png");
	game->textures->pause = mlx_load_png("./5_TEXTURES/0_UTILS/pause.png");
	if (!game->textures->east || !game->textures->west || !game->textures->pause
		|| !game->textures->north || !game->textures->door
		|| !game->textures->south || !game->textures->floor
		|| !game->textures->ceilling || !game->textures->icon)
	{
		ft_printf(RED"ERROR!\n"GRN"At least one image has not loaded\n"RST);
		return (0);
	}
	convert_image(game);
	return (1);
}

int	load_matrix(t_game *game)
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
