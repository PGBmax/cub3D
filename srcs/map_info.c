/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_info.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maregnie <maregnie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 16:46:13 by maregnie          #+#    #+#             */
/*   Updated: 2025/04/10 13:54:39 by maregnie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int check_info(t_game *game)
{
	if (game->info->north && game->info->south && game->info->west
		&& game->info->east && game->info->floor && game->info->ceiling)
		return (1);
	else
		return (0);

}


int	get_map_info(t_game *game, int i)
{
	t_map	*tmp;

	tmp = game->map;
	game->info = malloc(sizeof(t_info));
	if (!game->info)
		return (0);
	while (tmp)
	{
		if (!ft_strncmp(tmp->content, "NO ", 3) && !game->info->north)
			game->info->north = tmp->content;
		else if (!ft_strncmp(tmp->content, "SO ", 3) && !game->info->south)
			game->info->south = tmp->content;
		else if (!ft_strncmp(tmp->content, "WE ", 3) && !game->info->west)
			game->info->west = tmp->content;
		else if (!ft_strncmp(tmp->content, "EA ", 3) && !game->info->east)
			game->info->east = tmp->content;	
		else if (!ft_strncmp(tmp->content, "F ", 2) && !game->info->floor)
			game->info->floor = tmp->content;
		else if (!ft_strncmp(tmp->content, "C ", 2) && !game->info->ceiling)
			game->info->ceiling = tmp->content;
		else if (tmp->content[0] != '\n')
				break ;
		tmp = tmp->next;
		i++;
	}
	if (!check_info(game))
		return (0);
	game->info->start_index = i;
	ft_printf("no : %s\nso : %s\nwe : %s\nea : %s\nf : %s\nc : %s", game->info->north, game->info->south, game->info->west, game->info->east, game->info->floor, game->info->ceiling);
	return (1);
}
