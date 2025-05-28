/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frames.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 14:45:42 by pboucher          #+#    #+#             */
/*   Updated: 2025/05/28 14:45:42 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

static void	convert_frame(t_game *game, int i)
{
	game->sprite->frames[i] = mlx_texture_to_image(game->mlx,
			game->textures->frames[i]);
	mlx_resize_image(game->sprite->frames[i], (int)(WIDTH / 3.36f),
		(int)(HEIGHT / 1.2f));
	mlx_image_to_window(game->mlx, game->sprite->frames[i], 0,
		(int)(HEIGHT / 4));
	game->sprite->frames[i]->enabled = false;
	free(modif2);
}

int	load_frames(t_game *game)
{
	char	*path;
	char	*modif;
	char	*modif2;
	int		i;

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
		convert_frame(game, i);
	}
	return (1);
}
