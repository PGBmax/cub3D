/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 16:27:59 by pboucher          #+#    #+#             */
/*   Updated: 2025/06/17 10:35:57 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
	free_texture(game->textures->icon);
	if (game->sprite)
		free(game->sprite);
	if (game->textures)
		free(game->textures);
	if (game->info)
		free(game->info);
	if (game->ray)
		free(game->ray);
	if (game->player)
		free(game->player);
}
