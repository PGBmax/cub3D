/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maregnie <maregnie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:43:57 by pboucher          #+#    #+#             */
/*   Updated: 2025/04/03 17:09:30 by maregnie         ###   ########.fr       */
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
	game = ft_calloc(sizeof(t_game), 1);
	game->map = get_map_as_list(av[1]);
	game->tab = get_map_as_tab(game->map);
	
	
	
	
	
	
	if (is_map_valid(game->tab))
		ft_printf("valid map\n");
	else
		ft_printf("invalid map\n");
}
