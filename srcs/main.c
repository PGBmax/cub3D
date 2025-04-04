/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:43:57 by pboucher          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/04/04 14:27:48 by pboucher         ###   ########.fr       */
=======
/*   Updated: 2025/04/04 13:47:37 by maregnie         ###   ########.fr       */
>>>>>>> refs/remotes/origin/Mario-Kart-Master
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int parse_tab(char **tab, size_t i, size_t len, size_t check)
{
	// if (!edge_parsing(tab))
	// 	return (0);
	while (tab[++i])
	{
		ft_printf("%s", tab[i]);
		if (ft_strlen(tab[i]) > len)
		{
			check = len - 2;
			while (tab[i][++check] && tab[i][check] != '\n')
				if (tab[i][check] != '1')
					return (0);
		}
		else if (ft_strlen(tab[i]) < len)
		{
			check = ft_strlen(tab[i]) - 2;
			while (tab[i - 1][++check] && tab[i - 1][check] != '\n')
				if (tab[i - 1][check] != '1')
					return (0);
		}
		len = ft_strlen(tab[i]);
	}
	return (1);
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
	game->map = get_map_as_list(av[1]);
	game->tab = get_map_as_tab(game->map);
	if (!parse_tab(game->tab, 1, ft_strlen(game->tab[0]), 0))
		error_msg(NOT_CLOSE, av[1]);
	ft_printf("\n\n\n");
	ft_tabprint(game->tab, 0);
	// if (is_map_valid(game->tab))
	// 	ft_printf("valid map\n");
	// else
	// 	ft_printf("invalid map\n");
	ft_tabfree(game->tab, ft_tablen(game->tab));
	free(game);
}
