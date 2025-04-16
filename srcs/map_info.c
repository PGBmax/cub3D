#include "cub3d.h"

int check_info(t_game *game)
{
	char	**tab1;
	char	**tab2;
	int		i;
	
	i = 0;
	if (game->info->north && game->info->south && game->info->west
		&& game->info->east && game->info->floor && game->info->ceiling)
		{
			tab1 = malloc(ft_strlen(game->info->floor));
			tab2 = malloc(ft_strlen(game->info->ceiling));
			while (i < 3)
			{
				tab1 = ft_split(game->info->floor, ',');
				tab2 = ft_split(game->info->ceiling, ',');
				game->info->info[0][i] = ft_atoi(tab1[i]);
				game->info->info[1][i] = ft_atoi(tab2[i]);
				i++;
			}
			ft_tabfree(tab1, 3);
			ft_tabfree(tab2, 3);
			return (1);
		}
	else
		return (0);

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
			game->info->north = &tmp->content[2];
		else if (!ft_strncmp(tmp->content, "SO ", 3) && !game->info->south)
			game->info->south = &tmp->content[2];
		else if (!ft_strncmp(tmp->content, "WE ", 3) && !game->info->west)
			game->info->west = &tmp->content[2];
		else if (!ft_strncmp(tmp->content, "EA ", 3) && !game->info->east)
			game->info->east = &tmp->content[2];
		else if (!ft_strncmp(tmp->content, "F ", 2) && !game->info->floor)
			game->info->floor = &tmp->content[2];
		else if (!ft_strncmp(tmp->content, "C ", 2) && !game->info->ceiling)
			game->info->ceiling = &tmp->content[2];
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