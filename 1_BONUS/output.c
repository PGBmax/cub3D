/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:02:53 by pboucher          #+#    #+#             */
/*   Updated: 2025/05/23 16:28:57 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

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
