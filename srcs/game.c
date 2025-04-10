/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maregnie <maregnie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:40:15 by pboucher          #+#    #+#             */
/*   Updated: 2025/04/10 17:01:16 by maregnie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// void	game_put_pixel(t_game *game)
// {
	// char	**floor;
	// char	**ceiling;

	// ft_printf("F: %s\nC: %s\n", game->info->floor, game->info->ceiling);
	// floor = ft_split()
	// mlx_put_pixel(game->sprite.fc, 0, 0, 0X000000FF);
	// mlx_put_pixel(game->sprite.fc, 0, 1, 0XFFFFFFFF);
// }

void	ft_game(t_game *game)
{
	mlx_set_setting(MLX_STRETCH_IMAGE, true);
	game->mlx = mlx_init(WIDTH, HEIGHT, "cub3D", true);
	if (!game->mlx)
		error_msg(MLX_CANNOT_CREATE, NULL);
	game->textures.fc = mlx_load_png("./fc.png");
	if (!game->textures.fc)
		error_msg(FAIL_LOAD, NULL);
	game->sprite.fc = mlx_texture_to_image(game->mlx, game->textures.fc);
	// game_put_pixel(game);
	mlx_resize_image(game->sprite.fc, WIDTH, HEIGHT);
	mlx_image_to_window(game->mlx, game->sprite.fc, 0, 0);
	mlx_loop(game->mlx);
	mlx_terminate(game->mlx);
}
