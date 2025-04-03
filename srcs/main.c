/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:43:57 by pboucher          #+#    #+#             */
/*   Updated: 2025/04/03 19:28:44 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
	game->map = get_map_as_list(av[1]);
	game->tab = get_map_as_tab(game->map);
	ft_tabprint(game->tab, 0);
	if (is_map_valid(game->tab))
		ft_printf("valid map\n");
	else
		ft_printf("invalid map\n");
}
