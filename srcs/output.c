/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:02:53 by pboucher          #+#    #+#             */
/*   Updated: 2025/04/10 17:31:54 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	error_msg(char *str, char *detail)
{
	t_game *game;

	game = get_tgame();
	ft_printf(RED "ERROR!\n" GRN);
	if (!detail)
		ft_printf(str, PROG_NAME);
	else
		ft_printf(str, PROG_NAME, detail);
	ft_printf(RST);
	if (game)
	{
		if (game->tab)
			ft_tabfree(game->tab, ft_tablen(game->tab));
		if (game->map)
			ft_lstclear(&game->map, free);
		free(game);
	}
	exit(EXIT_FAILURE);
}
