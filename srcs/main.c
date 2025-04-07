/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:18:28 by pboucher          #+#    #+#             */
/*   Updated: 2025/04/07 15:55:13 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	edge_parsing(char **tab)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (tab[i])
	{
		j = 0;
		while (tab[i][j] == ' ' || tab[i][j] == '\t')
			j++;
		if (tab[i][j] != '1' || tab[i][ft_strlen(tab[i]) - 2] != '1')
			return (0);
		i++;
	}
	return (1);
}

int	first_line_parsing(char **tab)
{
	size_t	i = 1;
	size_t	j;
	size_t	check;
	while (tab[0][check] == ' ' || tab[0][check] == '\t')
		check++;
	while (tab[i])
	{
		j = 0;
		ft_printf("%s", tab[i]);
		while (tab[0][j] == ' ' || tab[0][j] == '\t')
			j++;
		if (j > check)
		{
			while(j > check)
			{
				if (tab[i][check] != '1')
					return (0);
				check++;
			}
			check = j;
		}
		if (j > check)
		{
			while(j > check)
			{
				if (tab[i][check] != '1')
					return (0);
				check++;
			}
		}
	}
}


int parse_tab(char **tab, size_t i, size_t len, size_t check)
{
	if (!edge_parsing(tab))
		return (0);
	if (!first_line_parsing(tab))
		retunr (0);
	ft_printf("\n\n\n");
	while (tab[++i])
	{
		ft_printf("%s", tab[i]);
		if (ft_strlen(tab[i]) > len)
		{
			check = len - 2;
			while (tab[i][++check] && tab[i][check] != '\n')
				if (tab[i][check] != '1')
					return (0);
		}
		else if (ft_strlen(tab[i]) < len)
		{
			check = ft_strlen(tab[i]) - 2;
			while (tab[i - 1][++check] && tab[i - 1][check] != '\n')
				if (tab[i - 1][check] != '1')
					return (0);
		}
		len = ft_strlen(tab[i]);
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
	if (!parse_tab(game->tab, 0, temp, 0))
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
