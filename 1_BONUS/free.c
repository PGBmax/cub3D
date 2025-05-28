/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 14:40:53 by pboucher          #+#    #+#             */
/*   Updated: 2025/05/28 14:40:59 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	free_texture(mlx_texture_t *texture)
{
	if (texture)
		mlx_delete_texture(texture);
}

void	free_game(t_game *game)
{
	free_texture(game->textures->east);
	free_texture(game->textures->west);
	free_texture(game->textures->north);
	free_texture(game->textures->south);
	free_texture(game->textures->pause);
	free_texture(game->textures->floor);
	free_texture(game->textures->ceilling);
	free_texture(game->textures->icon);
	if (game->sprite)
		free(game->sprite);
	if (game->textures)
		free(game->textures);
	if (game->info)
		free(game->info);
	if (game->r)
		free(game->r);
}
