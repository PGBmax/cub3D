/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_info.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 16:41:33 by pboucher          #+#    #+#             */
/*   Updated: 2025/05/27 14:08:47 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

int check_info(t_game *game)
{
	char	**tab1;
	char	**tab2;
	int		i;
	
	i = 0;
	if (game->info->north && game->info->south && game->info->west
		&& game->info->east && game->info->floor && game->info->ceiling)
		{
			tab1 = ft_split(game->info->floor, ',');
			tab2 = ft_split(game->info->ceiling, ',');
			
			while (i < RGBLEN && tab1[i] && tab2[i])
			{
				game->info->info[0][i] = ft_atoi(tab1[i]);
				game->info->info[1][i] = ft_atoi(tab2[i]);
				i++;
			}
			ft_freesplit(tab1);
			ft_freesplit(tab2);
			return (1);
		}
	else
		return (0);

}

char	*dup_cutendl(char *src, char *cpy)
{
	int i;

	i = 0;
	while (src[i] && src[i] != '\n')
	{
		cpy[i] = src[i];
		i++;
	}
	cpy[i] = 0;
	return (cpy);
}

int	get_map_info(t_game *game, int i)
{
	t_map	*tmp;
	tmp = game->map;
	game->info = ft_calloc(sizeof(t_info), 1);
	if (!game->info)
		return (0);
	while (tmp)
	{
		if (!ft_strncmp(tmp->content, "NO ", 3) && !game->info->north)
			game->info->north = dup_cutendl(&tmp->content[3], &tmp->content[3]);
		else if (!ft_strncmp(tmp->content, "SO ", 3) && !game->info->south)
			game->info->south = dup_cutendl(&tmp->content[3], &tmp->content[3]);
		else if (!ft_strncmp(tmp->content, "WE ", 3) && !game->info->west)
			game->info->west = dup_cutendl(&tmp->content[3], &tmp->content[3]);
		else if (!ft_strncmp(tmp->content, "EA ", 3) && !game->info->east)
			game->info->east = dup_cutendl(&tmp->content[3], &tmp->content[3]);
		else if (!ft_strncmp(tmp->content, "FL ", 3) && !game->info->floor)
			game->info->floor = dup_cutendl(&tmp->content[3], &tmp->content[3]);
		else if (!ft_strncmp(tmp->content, "CE ", 3) && !game->info->ceiling)
			game->info->ceiling = dup_cutendl(&tmp->content[3], &tmp->content[3]);
		else if (!ft_strncmp(tmp->content, "DO ", 3) && !game->info->door)
			game->info->door = dup_cutendl(&tmp->content[3], &tmp->content[3]);
		else if (tmp->content[0] != '\n')
				break ;
		tmp = tmp->next;
		i++;
	}
	if (!check_info(game))
		return (0);
	while (tmp->content[0] != '1' && tmp->content[0] != ' ')
	{
		tmp = tmp->next;
		i++;
	}
	game->info->start_index = i;
	return (1);
}
