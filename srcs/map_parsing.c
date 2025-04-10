/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:52:02 by pboucher          #+#    #+#             */
/*   Updated: 2025/04/10 15:26:12 by pboucher         ###   ########.fr       */
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
    i = -1;
	while (++i <= index)
		lstmap = lstmap->next;
    map = malloc(sizeof(char *) * (ft_lstsize(lstmap) + 1));
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

int parse_tab(char **tab)
{
	int i;
	int j;

	if (!edge_parsing(tab))
		return (0);
	i = 0;
	while (tab[i])
	{
		j = 0;
		// ft_printf("%s\n", tab[i]);
		while (tab[i][j])
		{
			// ft_printf(RED "i : %d, j : %d, tab[i][j] : %c\n" RST, i, j, tab[i][j]);
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
