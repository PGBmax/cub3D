/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_texture.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maregnie <maregnie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:26:07 by maregnie          #+#    #+#             */
/*   Updated: 2025/04/18 16:39:45 by maregnie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	get_textures(t_game *game)
{
	t_textures	*texture;

	texture = ft_calloc(sizeof(t_textures), 1);
	if (!texture)
		return (0);
	texture->wall = mlx_load_png("./shrek.png");
	game->textures = texture;
	if (!texture->wall)
		return (0);
	return (1);
}

int	convert_textures(t_game *game)
{
	t_sprite	*sprites;

	sprites = ft_calloc(sizeof(t_sprite), 1);
	if (!sprites)
		return (0);
	if (!get_textures(game))
	{
		free(sprites);
		return (0);
	}
	sprites->wall = mlx_texture_to_image(game->mlx, game->textures->wall);
	game->sprite = sprites;
	return (1);
}

int	put_textures(t_game *game, char elem, int x, int y)
{
	mlx_image_t	*tmp;

	if (elem == '1')
		tmp = game->sprite->wall;
	else
		return (-1);
	if ((mlx_image_to_window(game->mlx, tmp, x, y)) == -1)
		return (0);
	return (1);
}

int	place_textures(t_game *game)
{
	int	x;
	int	y;

	if (!convert_textures(game))
		return (0);
	y = 0;
	while (game->tab[y])
	{
		x = 0;
		while (game->tab[y][x])
		{
			if (!put_textures(game, game->tab[y][x], x, y))
				return (0);
			x++;
		}
		y++;
	}
	return (1);
}