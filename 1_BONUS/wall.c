/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 15:14:23 by pboucher          #+#    #+#             */
/*   Updated: 2025/05/28 14:14:52 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

uint32_t	get_color(mlx_image_t *img, int x, int y)
{
	t_game	*game;

	game = get_tgame();
	return ((uint32_t)((img->pixels[(y * game->s_width + x) * 4] << 24)
		| ((img->pixels[(y * game->s_width + x) * 4 + 1]) << 16)
		| ((img->pixels[(y * game->s_width + x) * 4 + 2]) << 8)
		| (img->pixels[(y * game->s_width + x) * 4 + 3])));
}

uint32_t	get_color2(mlx_image_t *img, int x, int y)
{
	t_game	*game;

	game = get_tgame();
	game->r->hit = 0;
	return ((uint32_t)((img->pixels[(y * game->s_width + x) * 4] << 24)
		| ((img->pixels[(y * game->s_width + x) * 4 + 1]) << 16)
		| ((img->pixels[(y * game->s_width + x) * 4 + 2]) << 8)
		| (img->pixels[(y * game->s_width + x) * 4 + 3]) / 2));
}

uint32_t	choose_wall(t_game *game, int texX, int texY)
{
	uint32_t	color;

	color = 0;
	if (game->tab[game->r->mapX][game->r->mapY] == '2')
	{
		color = get_color(game->sprite->door, texX, texY);
		return (color);
	}
	if (game->tab[game->r->mapX][game->r->mapY] == '3')
	{
		color = get_color2(game->sprite->door, texX, texY);
		return (color);
	}
	if (game->r->side == 0 && game->r->rayDirX <= 0)
		color = get_color(game->sprite->north, texX, texY);
	else if (game->r->side == 0 && game->r->rayDirX >= 0)
		color = get_color(game->sprite->south, texX, texY);
	else if (game->r->side == 1 && game->r->rayDirY <= 0)
		color = get_color2(game->sprite->west, texX, texY);
	else if (game->r->side == 1 && game->r->rayDirY >= 0)
		color = get_color2(game->sprite->east, texX, texY);
	return (color);
}

void	print_wall(t_game *game, int x)
{
	uint32_t	color;

	game->r->drawStart -= 1;
	while (++game->r->drawStart <= game->r->drawEnd)
	{
		game->r->texY = (int)game->r->texPos & (game->s_height - 1);
		game->r->texPos += game->r->step;
		color = choose_wall(game, game->r->texX, game->r->texY);
		mlx_put_pixel(game->screen, x, game->r->drawStart, color);
	}
}

void	draw_wall(t_game *game, int x)
{
	game->r->wallX = game->r->posX + game->r->perpWallDist * game->r->rayDirX;
	if (game->r->side == 0)
		game->r->wallX = game->r->posY + game->r->perpWallDist
			* game->r->rayDirY;
	game->r->wallX -= floor(game->r->wallX);
	game->r->texX = (int)(game->r->wallX * (float)game->s_width);
	if ((game->r->side == 0 && game->r->rayDirX > 0)
		|| (game->r->side == 1 && game->r->rayDirY < 0))
		game->r->texX = game->s_width - game->r->texX - 1;
	game->r->step = 1.0f * game->s_height / game->r->lineH;
	game->r->texPos = (game->r->drawStart - HEIGHT / 2 + game->r->lineH / 2)
		* game->r->step;
	print_wall(game, x);
}
