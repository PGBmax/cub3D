/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maregnie <maregnie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:18:28 by pboucher          #+#    #+#             */
/*   Updated: 2025/05/06 14:09:17 by maregnie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int main(int ac, char **av)
{
	t_game	*game;
	int 	fd;

	if (ac != 2)
		error_msg(CORRECT_USAGE, NULL);
	if (!is_valid(av[1]))
		error_msg(INVALID_TERM, av[1]);
	fd = open(av[1], O_RDONLY);
	if (fd == 1)
		error_msg(CANNOT_OPEN, av[1]);
	close(fd);
	game = ft_calloc(sizeof(t_game), 1);
	if (!game)
		error_msg(MALLOC_ERROR, NULL);
	set_tgame(game);
	game->map = get_map_as_list(av[1]);
	if (!get_map_info(game, 0))
	{
		if (game->info)
			free(game->info);
		ft_lstclear(&game->map, free);
		error_msg(MAP_INFO_FAIL, av[1]);
	}
	ft_printf("%d\n", game->info->start_index);
	game->tab = get_map_as_tab(game->map, game->info->start_index - 1);
	ft_tabprint(game->tab, 0);
	if (!parse_tab(game, 0, 0, 0))
	{
		free(game->info);
		error_msg(NOT_CLOSE, av[1]);
	}
	ft_game(game);
	free_game(game);
	free(game->player);
	ft_lstclear(&game->map, free);
	ft_tabfree(game->tab, ft_tablen(game->tab));
	free(game);
	close(fd);
}
