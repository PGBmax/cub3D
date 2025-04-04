/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maregnie <maregnie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 17:49:08 by maregnie          #+#    #+#             */
/*   Updated: 2025/04/04 11:29:34 by maregnie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	line_length(char *str)
{
	int i;

	i = 0;
	while (str[i] != '\n' && str[i] != '\0')
		i++;
	ft_printf("%d\n", i);
	return (i);
}

void	free_sprite(mlx_image_t	*sprite, t_game *game)
{
	if (sprite)
		mlx_delete_image(game->mlx, sprite);
}

void	free_texture(mlx_texture_t *texture)
{
	if (texture)
		mlx_delete_texture(texture);
}

void	free_textures(t_game *game)
{
	free_texture(game->textures->sky);
	free_texture(game->textures->npc);
	free_texture(game->textures->ground);
	free_texture(game->textures->wall);
	if (game->sprite)
	{
		free_sprite(game->sprite->ground, game);
		free_sprite(game->sprite->wall, game);
		free_sprite(game->sprite->sky, game);
		free_sprite(game->sprite->npc, game);
		free(game->sprite);
	}
	if (game->textures)
		free(game->textures);
}

void	ft_free(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
}