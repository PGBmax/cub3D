/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_management.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:03:53 by maregnie          #+#    #+#             */
/*   Updated: 2025/04/07 15:02:53 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	get_textures(t_game *game)
{
	t_textures	*texture;

	texture = ft_calloc(sizeof(t_textures), 1);
	if (!texture)
		return (0);
	texture->wall = mlx_load_png("./textures/wall.png");
	texture->ground = mlx_load_png("./textures/ground.png");
	texture->sky = mlx_load_png("./textures/sky.png");
	texture->npc = mlx_load_png("./textures/npc.png");
	game->textures = texture;
	if (!texture->wall || !texture->ground)
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
	sprites->ground = mlx_texture_to_image(game->mlx, game->textures->ground);
	sprites->sky = mlx_texture_to_image(game->mlx, game->textures->sky);
	sprites->npc = mlx_texture_to_image(game->mlx, game->textures->npc);
	game->sprite = sprites;
	return (1);
}

int	put_textures(t_game *game, char elem, int x, int y)
{
	mlx_image_t	*tmp;

	if (elem == '1')
		tmp = game->sprite->wall;
	else if (elem == 'N' || elem == '0' || elem == 'S' || elem == 'E'
		|| elem == 'W')
		tmp = game->sprite->ground;
	else
		return (-1);
	if ((mlx_image_to_window(game->mlx, tmp, x * 32, y * 32)) == -1)
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
	// if (mlx_image_to_window(game->mlx,
			// game->sprite->player, game->player_x * 32, game->player_y * 32)
		// == -1)
		return (0);
	return (1);
}