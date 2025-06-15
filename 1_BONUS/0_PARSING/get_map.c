/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_map.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 16:15:18 by pboucher          #+#    #+#             */
/*   Updated: 2025/06/15 16:20:13 by pboucher         ###   ########.fr       */
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

t_map	*get_map_as_list(char *argv)
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

char	**alloc_map(t_map *lstmap, t_map **tmp)
{
	char	**map;

	map = malloc(sizeof(char *) * (ft_lstsize(lstmap) + 4));
	if (!map)
	{
		ft_lstclear(tmp, free);
		error_msg(MALLOC_ERROR, NULL);
	}
	return (map);
}

char	**get_map_as_tab(t_map *lstmap, int index)
{
	t_map	*tmp;
	int		i;
	char	**map;

	tmp = lstmap;
	i = -1;
	while (++i <= index)
		lstmap = lstmap->next;
	map = alloc_map(lstmap, &tmp);
	i = 0;
	while (lstmap)
	{
		map[i] = ft_strdup(lstmap->content);
		lstmap = lstmap->next;
		i++;
	}
	longest_line(&map[i], 0);
	if (!mapverif(map, 0, 0))
	{
		ft_tabfree(map, ft_tablen(map) - 3);
		return (NULL);
	}
	return (map);
}
