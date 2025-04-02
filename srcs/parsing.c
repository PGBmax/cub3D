/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:26:07 by pboucher          #+#    #+#             */
/*   Updated: 2025/04/01 18:24:03 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

bool	is_valid(char *path)
{
	int	i;

	i = ft_strlen(path);
	if (i < 4)
		return (false);
	if (ft_strcmp(&path[i - 4], ".cub"))
		return (false);
	if (!open(path))
		return (false);
	return (true);
}
