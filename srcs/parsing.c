/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maregnie <maregnie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:26:07 by pboucher          #+#    #+#             */
/*   Updated: 2025/04/03 17:30:20 by maregnie         ###   ########.fr       */
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
	if (!open(path, O_RDONLY))
		return (false);
	return (true);
}
int	is_map_valid(char **map)
{
	int i;
	int j;
	
	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j] && map[i][j] != 12)
		{
			while (map[i][j] == 32 || (map[i][j]) == 9)
				j++;
			if (map[i][j] != '1' && map[i][j] != '0' && map[i][j] != 'N'
				&& map[i][j] != 'S' && map[i][j] != 'W' && map[i][j] != 'E'
				&& map[i][j] != ' ' && map[i][j] != 10)
			{
				// ft_printf("%c", map[i][j]);
				return (0);
			}	
			j++;
		}
		i++;
	}
	return (1);
}
