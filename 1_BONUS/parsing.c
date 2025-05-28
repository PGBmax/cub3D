/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:26:07 by pboucher          #+#    #+#             */
/*   Updated: 2025/05/28 14:19:56 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

bool	is_valid(char *path)
{
	int	i;
	int	fd;

	i = ft_strlen(path);
	if (i < 4)
		return (false);
	if (ft_strcmp(&path[i - 4], ".cub"))
		return (false);
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (false);
	close(fd);
	return (true);
}

char	*void_changer(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != 10)
	{
		if (str[i] == ' ' || str[i] == '\t')
			str[i] = '1';
		i++;
	}
	return (str);
}
