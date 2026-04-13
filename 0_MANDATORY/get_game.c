/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_game.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 17:15:32 by pboucher          #+#    #+#             */
/*   Updated: 2025/04/08 17:16:39 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
