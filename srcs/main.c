/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:18:28 by pboucher          #+#    #+#             */
/*   Updated: 2025/04/10 14:06:17 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	ft_game(t_game *game)
{
	mlx_set_setting(MLX_STRETCH_IMAGE, true);
	game->mlx = mlx_init(WIDTH, HEIGHT, "cub3D", true);
	if (!game->mlx)
		error_msg(MLX_CANNOT_CREATE, NULL);
	game->textures->fc = mlx_load_png("./fc.png");
	if (!game->textures->fc)
		error_msg(FAIL_LOAD, NULL);
	game->sprite->fc = mlx_texture_to_image(game->mlx, game->textures->fc);
	mlx_resize_image(game->sprite->fc, WIDTH, HEIGHT);
	mlx_image_to_window(game->mlx, game->sprite->fc, 0, 0);
	mlx_loop(game->mlx);
	mlx_terminate(game->mlx);
	ft_tabfree(game->tab, ft_tablen(game->tab));
	free(game);
}


int main(int ac, char **av)
{
	t_game	*game;

	if (ac != 2)
		error_msg(CORRECT_USAGE, NULL);
	if (!is_valid(av[1]))
		error_msg(INVALID_TERM, av[1]);
	if (open(av[1], O_RDONLY) == -1)
		error_msg(CANNOT_OPEN, av[1]);
	game = ft_calloc(sizeof(t_game), 1);
	if (!game)
		error_msg(MALLOC_ERROR, NULL);
	set_tgame(game);
	game->map = get_map_as_list(av[1]);
	game->tab = get_map_as_tab(game->map);
	if (!parse_tab(game->tab))
		error_msg(NOT_CLOSE, av[1]);
	ft_game(game);
}
