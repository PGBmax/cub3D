/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:52:02 by pboucher          #+#    #+#             */
/*   Updated: 2025/04/04 14:24:48 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_map    *get_map_as_list(char *argv)
{
    t_map    *map;
    char    *tmp;
    int        fd;

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

char    **get_map_as_tab(t_map *lstmap)
{
    t_map    *tmp;
    int        i;
    char    **map;

    tmp = lstmap;
    i = 0;
    map = malloc(sizeof(char *) * (ft_lstsize(lstmap) + 1));
    if (!map)
    {
        ft_lstclear(&tmp, free);
        return (NULL);
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
