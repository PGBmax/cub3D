/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   effects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 00:00:00 by pboucher          #+#    #+#             */
/*   Updated: 2026/04/15 14:56:26 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

/*
** Dessine un petit crosshair ('+') au centre de l'écran.
** 2 lignes de CROSS_SIZE pixels, blanc semi-transparent.
*/
void	draw_crosshair(t_game *game)
{
	int			i;
	int			cx;
	int			cy;
	uint32_t	c;

	cx = WIDTH / 2;
	cy = HEIGHT / 2;
	c = 0xFFFFFF99;
	i = -CROSS_SIZE - 1;
	while (++i <= CROSS_SIZE)
	{
		if (i != 0)
		{
			mlx_put_pixel(game->screen, cx + i, cy, c);
			mlx_put_pixel(game->screen, cx, cy + i, c);
		}
	}
	mlx_put_pixel(game->screen, cx, cy, 0xFFFFFFDD);
}

/*
** Effet vignette : assombrit les pixels proches du bord de l'écran.
** On parcourt uniquement la bordure (bande de VIGNETTE_R pixels).
** L'intensité est fonction de la distance au bord.
*/
static uint32_t	darken_pixel(uint32_t color, float factor)
{
	int	r;
	int	g;
	int	b;
	int	a;

	r = (int)((float)((color >> 24) & 0xFF) * factor);
	g = (int)((float)((color >> 16) & 0xFF) * factor);
	b = (int)((float)((color >> 8) & 0xFF) * factor);
	a = color & 0xFF;
	return ((uint32_t)(r << 24 | g << 16 | b << 8 | a));
}

static float	vignette_factor(int x, int y)
{
	float	dx;
	float	dy;
	float	d;

	dx = (float)(x - WIDTH / 2) / (float)(WIDTH / 2);
	dy = (float)(y - HEIGHT / 2) / (float)(HEIGHT / 2);
	d = sqrtf(dx * dx + dy * dy);
	d = (d - VIGNETTE_START) / (VIGNETTE_END - VIGNETTE_START);
	if (d < 0.0f)
		return (1.0f);
	if (d > 1.0f)
		d = 1.0f;
	return (1.0f - d * VIGNETTE_STRENGTH);
}

void	draw_vignette(t_game *game)
{
	int			x;
	int			y;
	uint32_t	px;
	float		f;

	y = -1;
	while (++y < HEIGHT)
	{
		x = -1;
		while (++x < WIDTH)
		{
			f = vignette_factor(x, y);
			if (f < 1.0f)
			{
				px = ((uint32_t)game->screen->pixels[(y * WIDTH + x) * 4]
					<< 24)
					| ((uint32_t)game->screen->pixels[(y * WIDTH + x)
						* 4 + 1] << 16)
					| ((uint32_t)game->screen->pixels[(y * WIDTH + x)
						* 4 + 2] << 8)
					| (uint32_t)game->screen->pixels[(y * WIDTH + x)
					* 4 + 3];
				mlx_put_pixel(game->screen, x, y, darken_pixel(px, f));
			}
		}
	}
}

/*
** Head bobbing : retourne un offset vertical en pixels
** basé sur le temps et la vitesse de marche.
** Retourne 0 si le joueur ne bouge pas.
*/
float	get_head_bob(t_game *game)
{
	if (!game->is_moving)
		return (0.0f);
	return (sinf((float)mlx_get_time() * BOB_SPEED) * BOB_AMOUNT);
}
