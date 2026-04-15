/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 00:00:00 by pboucher          #+#    #+#             */
/*   Updated: 2026/04/15 15:19:08 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"
#include <pthread.h>

/*
** Chaque thread possède un range exclusif de colonnes X [x_start, x_end).
** Il dessine sol/plafond PUIS murs uniquement pour ses colonnes.
** Aucun chevauchement entre threads → zéro race condition.
*/

static void	thr_init_ray(t_thread_data *td, int x)
{
	td->ray.cameraX = 2 * x / (float)WIDTH - 1;
	td->ray.rayDirX = td->ray.dirX + td->ray.planeX * td->ray.cameraX;
	td->ray.rayDirY = td->ray.dirY + td->ray.planeY * td->ray.cameraX;
	td->ray.mapX = (int)td->ray.posX;
	td->ray.mapY = (int)td->ray.posY;
	if (td->ray.rayDirX == 0)
		td->ray.deltaDistX = 1e30;
	else
		td->ray.deltaDistX = fabsf(1.0f / td->ray.rayDirX);
	if (td->ray.rayDirY == 0)
		td->ray.deltaDistY = 1e30;
	else
		td->ray.deltaDistY = fabsf(1.0f / td->ray.rayDirY);
	td->ray.hit = 0;
}

static void	thr_update_step(t_ray *ray)
{
	ray->stepX = 1;
	ray->sideDistX = (ray->mapX + 1.0f - ray->posX) * ray->deltaDistX;
	if (ray->rayDirX < 0)
	{
		ray->stepX = -1;
		ray->sideDistX = (ray->posX - ray->mapX) * ray->deltaDistX;
	}
	ray->stepY = 1;
	ray->sideDistY = (ray->mapY + 1.0f - ray->posY) * ray->deltaDistY;
	if (ray->rayDirY < 0)
	{
		ray->stepY = -1;
		ray->sideDistY = (ray->posY - ray->mapY) * ray->deltaDistY;
	}
}

static void	thr_has_touch(t_thread_data *td)
{
	while (td->ray.hit == 0)
	{
		if (td->ray.sideDistX < td->ray.sideDistY)
		{
			td->ray.sideDistX += td->ray.deltaDistX;
			td->ray.mapX += td->ray.stepX;
			td->ray.side = 0;
		}
		else
		{
			td->ray.sideDistY += td->ray.deltaDistY;
			td->ray.mapY += td->ray.stepY;
			td->ray.side = 1;
		}
		if (td->game->tab[td->ray.mapX][td->ray.mapY] != '0')
			td->ray.hit = 1;
	}
}

static uint32_t	thr_choose_wall(t_thread_data *td, int texX, int texY)
{
	t_matrix	*m;

	m = td->game->matrix;
	if (td->game->tab[td->ray.mapX][td->ray.mapY] == '2')
		return (m->door[texX][texY]);
	if (td->game->tab[td->ray.mapX][td->ray.mapY] == '3')
		return (m->door[texX][texY] - 128);
	if (td->ray.side == 0 && td->ray.rayDirX <= 0)
		return (m->north[texX][texY]);
	if (td->ray.side == 0 && td->ray.rayDirX >= 0)
		return (m->south[texX][texY]);
	if (td->ray.side == 1 && td->ray.rayDirY <= 0)
		return (m->west[texX][texY]);
	return (m->east[texX][texY]);
}

static void	thr_print_wall(t_thread_data *td, int x)
{
	uint32_t	color;

	td->ray.drawStart -= 1;
	while (++td->ray.drawStart <= td->ray.drawEnd)
	{
		td->ray.texY = (int)td->ray.texPos & (S_BOX - 1);
		td->ray.texPos += td->ray.step;
		color = thr_choose_wall(td, td->ray.texX, td->ray.texY);
		color = fog_color(color, td->ray.perpWallDist);
		mlx_put_pixel(td->game->screen, x, td->ray.drawStart, color);
	}
}

static void	thr_draw_wall(t_thread_data *td, int x)
{
	td->ray.wallX = td->ray.posX
		+ td->ray.perpWallDist * td->ray.rayDirX;
	if (td->ray.side == 0)
		td->ray.wallX = td->ray.posY + td->ray.perpWallDist
			* td->ray.rayDirY;
	td->ray.wallX -= floorf(td->ray.wallX);
	td->ray.texX = (int)(td->ray.wallX * (float)S_BOX);
	if ((td->ray.side == 0 && td->ray.rayDirX > 0)
		|| (td->ray.side == 1 && td->ray.rayDirY < 0))
		td->ray.texX = S_BOX - td->ray.texX - 1;
	td->ray.step = 1.0f * S_BOX / td->ray.lineH;
	td->ray.texPos = (td->ray.drawStart - HEIGHT / 2
			+ td->ray.lineH / 2) * td->ray.step;
	thr_print_wall(td, x);
}

static void	render_walls_slice(t_thread_data *td)
{
	int	x;

	x = td->x_start - 1;
	while (++x < td->x_end)
	{
		thr_init_ray(td, x);
		thr_update_step(&td->ray);
		thr_has_touch(td);
		if (td->ray.side == 0)
			td->ray.perpWallDist = td->ray.sideDistX - td->ray.deltaDistX;
		else
			td->ray.perpWallDist = td->ray.sideDistY - td->ray.deltaDistY;
		td->ray.lineH = (int)(HEIGHT / td->ray.perpWallDist);
		td->ray.drawStart = (int)(-td->ray.lineH / 2 + HEIGHT / 2 + td->bob);
		if (td->ray.drawStart < 0)
			td->ray.drawStart = 0;
		td->ray.drawEnd = (int)(td->ray.lineH / 2 + HEIGHT / 2 + td->bob);
		if (td->ray.drawEnd >= HEIGHT)
			td->ray.drawEnd = HEIGHT - 1;
		thr_draw_wall(td, x);
	}
}

/*
** Initialise les paramètres sol/plafond pour une ligne Y donnée.
** Saute directement à x_start via multiplication.
*/
static void	init_floor_row(t_thread_data *td, int y)
{
	t_ray	*gr;

	gr = td->game->ray;
	td->ray.rayDirX0 = gr->dirX - gr->planeX;
	td->ray.rayDirY0 = gr->dirY - gr->planeY;
	td->ray.rayDirX1 = gr->dirX + gr->planeX;
	td->ray.rayDirY1 = gr->dirY + gr->planeY;
	td->ray.p = y - HEIGHT / 2;
	if (td->ray.p == 0)
		td->ray.p = 1;
	td->ray.posZ = 0.5f * HEIGHT;
	td->ray.rowDistance = td->ray.posZ / (float)td->ray.p;
	td->ray.floorStepX = td->ray.rowDistance
		* (td->ray.rayDirX1 - td->ray.rayDirX0) / WIDTH;
	td->ray.floorStepY = td->ray.rowDistance
		* (td->ray.rayDirY1 - td->ray.rayDirY0) / WIDTH;
	td->ray.floorX = gr->posX + td->ray.rowDistance * td->ray.rayDirX0
		+ td->x_start * td->ray.floorStepX;
	td->ray.floorY = gr->posY + td->ray.rowDistance * td->ray.rayDirY0
		+ td->x_start * td->ray.floorStepY;
}

/*
** Dessine les pixels sol et plafond pour une ligne Y
** uniquement pour les colonnes [x_start, x_end).
** Fog uniforme appliqué avec la même distance que les murs.
*/
static void	draw_floor_pixels(t_thread_data *td, int y)
{
	int			x;
	uint32_t	fc;
	uint32_t	cc;

	x = td->x_start - 1;
	while (++x < td->x_end)
	{
		td->ray.tx = (int)(S_BOX
				* (td->ray.floorX - (int)td->ray.floorX)) & (S_BOX - 1);
		td->ray.ty = (int)(S_BOX
				* (td->ray.floorY - (int)td->ray.floorY)) & (S_BOX - 1);
		fc = td->game->matrix->floor[td->ray.tx][td->ray.ty];
		cc = td->game->matrix->ceilling[td->ray.tx][td->ray.ty];
		mlx_put_pixel(td->game->screen, x, y,
			fog_color(fc, td->ray.rowDistance));
		mlx_put_pixel(td->game->screen, x, HEIGHT - y - 1,
			fog_color(cc, td->ray.rowDistance));
		td->ray.floorX += td->ray.floorStepX;
		td->ray.floorY += td->ray.floorStepY;
	}
}

static void	render_floor_ceil_cols(t_thread_data *td)
{
	int	y;

	y = HEIGHT / 2;
	while (++y < HEIGHT)
	{
		init_floor_row(td, y);
		draw_floor_pixels(td, y);
	}
}

static void	*thread_render(void *arg)
{
	t_thread_data	*td;

	td = (t_thread_data *)arg;
	render_floor_ceil_cols(td);
	render_walls_slice(td);
	return (NULL);
}

/*
** Divise le rendu en NUM_THREADS tranches verticales de colonnes.
** Chaque thread a un range exclusif de colonnes → pas de race condition.
*/
void	draw_ray_threaded(t_game *game)
{
	pthread_t		threads[NUM_THREADS];
	t_thread_data	td[NUM_THREADS];
	int				slice_w;
	int				i;

	slice_w = WIDTH / NUM_THREADS;
	i = -1;
	while (++i < NUM_THREADS)
	{
		td[i].game = game;
		td[i].ray = *game->ray;
		td[i].bob = get_head_bob(game);
		td[i].x_start = i * slice_w;
		if (i == NUM_THREADS - 1)
			td[i].x_end = WIDTH;
		else
			td[i].x_end = (i + 1) * slice_w;
		pthread_create(&threads[i], NULL, thread_render, &td[i]);
	}
	i = -1;
	while (++i < NUM_THREADS)
		pthread_join(threads[i], NULL);
}
