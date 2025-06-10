/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 16:28:10 by pboucher          #+#    #+#             */
/*   Updated: 2025/06/10 14:15:37 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	free_matrix(uint32_t **matrix)
{
	int i;

	i = 0;
	while (i < S_BOX)
	{
		free(matrix[i]);
		i++;
	}
	free(matrix);
}

void	free_texture(mlx_texture_t *texture)
{
	if (texture)
		mlx_delete_texture(texture);
}

void	free_game(t_game *game)
{
	int i;

	i = 0;
	free_texture(game->textures->east);
	free_texture(game->textures->west);
	free_texture(game->textures->north);
	free_texture(game->textures->south);
	free_texture(game->textures->floor);
	free_texture(game->textures->door);
	free_texture(game->textures->ceilling);
	free_texture(game->textures->icon);
	free_matrix(game->matrix->east);
	free_matrix(game->matrix->north);
	free_matrix(game->matrix->south);
	free_matrix(game->matrix->west);
	free_matrix(game->matrix->floor);
	free_matrix(game->matrix->ceilling);
	free_matrix(game->matrix->door);
	while (i < 44)
	{
		free_texture(game->textures->frames[i]);
		i++;
	}
	free(game->sprite);
	free(game->textures);
	free(game->matrix);
	free(game->player);
	free(game->ray);
	free(game->info);
}
