/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:18:28 by pboucher          #+#    #+#             */
/*   Updated: 2025/04/08 16:18:13 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
		// ft_printf("%s", tab[i]);
		while (tab[i][j])
		{
			// ft_printf(RED"i : %d, j : %d, tab[i][j] : %c\n"RST, i, j, tab[i][j]);
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

int main(int ac, char **av)
{
	t_game	*game;
	size_t	temp;
	
	if (ac != 2)
		error_msg(CORRECT_USAGE, NULL);
	if (!is_valid(av[1]))
		error_msg(INVALID_TERM, av[1]);
	if (open(av[1], O_RDONLY) == -1)
		error_msg(CANNOT_OPEN, av[1]);
	game = ft_calloc(sizeof(t_game), 1);
	if (!game)
		error_msg(MALLOC_ERROR, NULL);
	game->map = get_map_as_list(av[1]);
	game->tab = get_map_as_tab(game->map);
	temp = ft_strlen(game->tab[0]);
	if (!parse_tab(game->tab))
		error_msg(NOT_CLOSE, av[1]);
	ft_printf("\n\n\n");
	ft_tabprint(game->tab, 0);
	// if (is_map_valid(game->tab))
	// 	ft_printf("valid map\n");
	// else
	// 	ft_printf("invalid map\n");
	ft_tabfree(game->tab, ft_tablen(game->tab));
	free(game);
}
