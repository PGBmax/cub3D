/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_game.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 17:15:32 by pboucher          #+#    #+#             */
/*   Updated: 2025/05/23 16:28:57 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

static t_game	*tgame(t_game *info)
{
	static t_game	*data = NULL;

	if (info)
		data = info;
	return (data);
}

void	set_tgame(t_game *info)
{
	tgame(info);
}

t_game	*get_tgame(void)
{
	return (tgame(NULL));
}
