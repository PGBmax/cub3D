/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maregnie <maregnie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:52:02 by pboucher          #+#    #+#             */
/*   Updated: 2025/04/10 14:05:14 by maregnie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
    i = 0;
	while (i <= index)
	{
		i++;
		lstmap = lstmap->next;
	}
	i = 0;
    map = malloc(sizeof(char *) * (ft_lstsize(lstmap) + 1));
    if (!map)
    {
        ft_lstclear(&tmp, free);
        error_msg(MALLOC_ERROR, NULL);
    }
    while (lstmap)
    {
        map[i] = ft_strdup(lstmap->content);
        lstmap = lstmap->next;
		i++;
    }
	map[i] = 0;
    ft_lstclear(&tmp, free);
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
	int	j;

	j = -1;
	while (tab[0][++j])
		if (tab[0][j] != '1'
			&& tab[0][j] != ' '
			&& tab[0][j] != '\t'
			&& tab[0][j] != '\n'
			&& tab[0][j] != '\0')
			return (0);
	j = -1;
	while (tab[ft_tablen(tab) - 1][++j])
		if (tab[ft_tablen(tab) - 1][j] != '1'
			&& tab[ft_tablen(tab) - 1][j] != ' '
			&& tab[ft_tablen(tab) - 1][j] != '\t'
			&& tab[ft_tablen(tab) - 1][j] != '\n'
			&& tab[ft_tablen(tab) - 1][j] != '\0')
			return (0);
	return (1);
}

int parse_tab(char **tab)
{
	int	i;
	int j;

	i = 0;
	if (!edge_parsing(tab))
		return (0);
	while (tab[i])
	{
		j = 0;
		while (tab[i][j])
		{
			if (tab[i][j] != '1' && tab[i][j] != ' ' && tab[i][j] != '\t'
				&& tab[i][j] != '\n' && tab[i][j] != '\0')
				if (!check_hole(tab, i, j))
					return (0);
			j++;
		}
		i++;
	}
	return (1);
}
