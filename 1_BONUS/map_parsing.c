/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:52:02 by pboucher          #+#    #+#             */
/*   Updated: 2025/05/23 16:28:57 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

char	**longest_line(char **tab, int i)
{
	while (i < 3)
	{
		tab[i] = "1";
		i++;
	}
	tab[i] = 0;
	return (tab);
}

t_map    *get_map_as_list(char *argv)
{
    t_map		*map;
    char		*tmp;
    int			fd;

    fd = open(argv, O_RDONLY);
    if (fd < 0)
    {    
		error_msg(INVALID_NAME, argv);
		return (NULL);
	}
    map = NULL;
    while (!map || tmp)
    {
        tmp = get_next_line(fd);
        if (!tmp)
            break ;
        if (!map)
            map = ft_lstnew(tmp);
        else
            ft_lstadd_back(&map, ft_lstnew(tmp));
    }
    free(tmp);
    close(fd);
    return (map);
}

char    **get_map_as_tab(t_map *lstmap, int index)
{
    t_map	*tmp;
    int		i;
    char	**map;

    tmp = lstmap;
    i = -1;
	while (++i <= index)
		lstmap = lstmap->next;
    map = malloc(sizeof(char *) * (ft_lstsize(lstmap) + 4));
    if (!map)
    {
        ft_lstclear(&tmp, free);
        error_msg(MALLOC_ERROR, NULL);
    }
	i = 0;
    while (lstmap)
    {
        map[i] = ft_strdup(lstmap->content);
        lstmap = lstmap->next;
		i++;
    }
	longest_line(&map[i], 0);
	if (!mapverif(map, 0, 0))
		return (NULL);
    return (map);
}

int check_hole(char **tab, int i, int j)
{
	int len_tab;
	int len_str;

	len_tab = ft_tablen(tab) - 1;
	len_str = ft_strlen(tab[i]) - 1;
	if (i != 0)
		if (!tab[i - 1][j] || tab[i - 1][j] == ' ' || tab[i - 1][j] == '\t'
			|| tab[i - 1][j] == '\n' || tab[i - 1][j] == '\0')
			return (0);
	if (i != len_tab)
		if (!tab[i + 1][j] || tab[i + 1][j] == ' ' || tab[i + 1][j] == '\t'
			|| tab[i + 1][j] == '\n' || tab[i + 1][j] == '\0')
			return (0);
	if (j != 0)
		if (!tab[i][j - 1] || tab[i][j - 1] == ' ' || tab[i][j - 1] == '\t'
			|| tab[i][j - 1] == '\n' || tab[i][j - 1] == '\0')
			return (0);
	if (j != len_str)
		if (!tab[i][j - 1] || tab[i][j + 1] == ' ' || tab[i][j + 1] == '\t'
			|| tab[i][j + 1] == '\n' || tab[i][j + 1] == '\0')
				return (0);
	return (1);
}

int edge_parsing(char **tab)
{
	int	i;

	i = -1;
	while (tab[0][++i])
		if (tab[0][i] != '1'
			&& tab[0][i] != ' '
			&& tab[0][i] != '\t'
			&& tab[0][i] != '\n'
			&& tab[0][i] != '\0')
			return (0);
	i = -1;
	while (tab[ft_tablen(tab) - 1][++i])
		if (tab[ft_tablen(tab) - 1][i] != '1'
			&& tab[ft_tablen(tab) - 1][i] != ' '
			&& tab[ft_tablen(tab) - 1][i] != '\t'
			&& tab[ft_tablen(tab) - 1][i] != '\n'
			&& tab[ft_tablen(tab) - 1][i] != '\0')
			return (0);
	return (1);
}

int parse_tab(t_game *game, int i, int j, int _bool)
{
	if (!edge_parsing(game->tab))
		return (0);
	while (game->tab[i])
	{
		j = 0;
		while (game->tab[i][j])
		{
			if (game->tab[i][j] != '1' && game->tab[i][j] != ' ' && game->tab[i][j] != '\t'
				&& game->tab[i][j] != '\n' && game->tab[i][j] != '\0')
			if (!check_hole(game->tab, i, j))
				return (0);
			if (game->tab[i][j] == 'S' || game->tab[i][j] == 'W' 
				|| game->tab[i][j] == 'N' || game->tab[i][j] == 'E')
			{
				if (_bool == 1)
					return (0);
				_bool = 1;
				game->info->pos = game->tab[i][j];
			}
			j++;
		}
		i++;
	}
	if (_bool != 1)
		return (0);
	return (1);
}