/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 17:49:08 by maregnie          #+#    #+#             */
/*   Updated: 2025/04/10 14:11:59 by pboucher         ###   ########.fr       */
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
	free_texture(game->textures->wall);
	free_texture(game->textures->fc);
	if (game->sprite)
	{
		free_sprite(game->sprite->wall, game);
		free_sprite(game->sprite->fc, game);
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