/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:40:15 by pboucher          #+#    #+#             */
/*   Updated: 2025/05/12 15:37:58 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <stdio.h>

uint32_t rgb_to_hex32(int *rgb)
{
    return ((uint32_t)((rgb[0] << 24) | (rgb[1] << 16) | (rgb[2] << 8) | 255));
}
uint32_t rgb_to_hex32v2(uint8_t *rgb)
{
    return ((uint32_t)((rgb[0] << 24) | (rgb[1] << 16) | (rgb[2] << 8) | rgb[3]));
}

// Vérifie si une position donnée (px, py) touche un mur ou sort des limites de la carte
int touch(float px, float py, t_game *game)
{
    int x1;
    int y1;
    int x2;
    int y2;

    // Convertit les coordonnées en indices de grille
    x1 = px / SIZE;
    y1 = py / SIZE;
    x2 = game->player->x / SIZE;
    y2 = game->player->y / SIZE;

    // Vérifie si la position est hors des limites ou si elle touche un mur
    if (y1 < 0 || y1 >= game->mapsize[1] || x1 < 0 || x1 >= game->mapsize[0] || game->tab[y1][x1] != game->tab[y2][x2])
        return (1);
    return (0);
}

// Calcule la distance euclidienne entre deux points
float distance(float x, float y)
{
    return (sqrt(x * x + y * y));
}

// Calcule une distance corrigée pour éviter l'effet "fish-eye"
float fixed_dist(t_game *game, t_ray *ray)
{
    float delta_x;
    float delta_y;
    float angle;
    float fixed_dist;

    // Différences entre la position du joueur et celle du rayon
    delta_x = ray->DirX - game->player->x;
    delta_y = ray->DirY - game->player->y;

    // Calcule l'angle entre le rayon et la direction du joueur
    angle = atan2(delta_y, delta_x) - game->player->a;

    // Corrige la distance en fonction de l'angle pour éviter l'effet "fish-eye"
    fixed_dist = distance(delta_x, delta_y) * cos(angle) * 0.55f;

    // Affiche la distance corrigée pour le débogage
    //printf("%f\n", fixed_dist);
    return (fixed_dist);
}

// Dessine une ligne verticale représentant un mur pour un rayon donné
void draw_line(t_game *game, float start_x, int i)
{
    t_ray ray;
    float cos_angle;
    float sin_angle;
    float dist;
    float height;
    float start_y;
    float end;
    int index;
    static float temp = 0.f;

    // Initialise la position du rayon à celle du joueur
    ray.DirX = game->player->x;
    ray.DirY = game->player->y;

    // Calcule les composantes directionnelles du rayon
    cos_angle = cos(start_x);
    sin_angle = sin(start_x);

    // Avance le rayon jusqu'à ce qu'il touche un mur
    while (!touch(ray.DirX, ray.DirY, game))
    {
        ray.DirX += cos_angle;
        ray.DirY += sin_angle;
    }
    // Calcule la distance corrigée et la hauteur du mur
    dist = fixed_dist(game, &ray);
    height = (SIZE / dist) * (WIDTH / 2.f);

    // Calcule les positions de début et de fin de la ligne verticale
    start_y = (HEIGHT - height) / 2.f;
    end = start_y + height;

    float save = start_y;
    // Dessine la ligne verticale pixel par pixel
    while (start_y < end && start_y < HEIGHT)
    {
        index = ((int)(start_y - save) * ((int)(end - start_y) % SIZE));
        uint32_t color = rgb_to_hex32v2(&game->sprite->north->pixels[index]);
        if (start_y >= 0)
            mlx_put_pixel(game->screen, i, start_y, color);
        start_y++;
        if (game->player->a != temp)
        {
            printf("%f\n", game->player->a);
            temp = game->player->a;
        }
    }
}

// Dessine tous les rayons pour générer la vue 3D
void draw_ray(t_game *game)
{
    int i;
    float frac;
    float start_x;

    // Initialise les variables pour parcourir les rayons
    i = 0;
    frac = PI / 3.f / WIDTH; // Angle entre chaque rayon
    start_x = game->player->a - PI / (2.f * PI); // Angle initial du premier rayon

    // Parcourt chaque colonne de l'écran
    while (i < WIDTH)
    {
        draw_line(game, start_x, i); // Dessine une ligne pour chaque rayon
        start_x += frac; // Passe au rayon suivant
        i++;
    }
}

// Dessine la carte en 2D et place le joueur si nécessaire
void draw_map(t_game *game, int check)
{
    int y;
    int x;

    y = 0;
    // Réinitialise l'image de la carte
    mlx_resize_image(game->screen, 1, 1);
    mlx_put_pixel(game->screen, 0, 0, 0x00000000);
    mlx_resize_image(game->screen, WIDTH, HEIGHT);

    // Parcourt la carte pour dessiner les éléments
    while (game->tab[y])
    {
        x = 0;
        while (game->tab[y][x + 1])
        {
			if (game->tab[y][x] == game->info->pos && check == 1)
			{
				game->player->x = x * SIZE + SIZE / 2.f;
				game->player->y = y * SIZE + SIZE / 2.f;
				game->tab[y][x] = '0';
			}
            x++;
        }
        y++;
    }
}

// Gère les mouvements du joueur en fonction des touches pressées
void make_move(t_game *game, double dx, double dy)
{
    float vector_length;

    vector_length = sqrt(pow(dx, 2) + pow(dy, 2));
    // Accélère le mouvement si la touche Shift est enfoncée
    if ((mlx_is_key_down(game->mlx, MLX_KEY_LEFT_SHIFT)
        || mlx_is_key_down(game->mlx, MLX_KEY_RIGHT_SHIFT))
        && (!mlx_is_key_down(game->mlx, MLX_KEY_LEFT_CONTROL)
        && !mlx_is_key_down(game->mlx, MLX_KEY_RIGHT_CONTROL)))
    {
        dx *= 1.5f;
        dy *= 1.5f;
    }

    // Ralentit le mouvement si la touche Control est enfoncée
    if ((mlx_is_key_down(game->mlx, MLX_KEY_LEFT_CONTROL)
        || mlx_is_key_down(game->mlx, MLX_KEY_RIGHT_CONTROL))
        && (!mlx_is_key_down(game->mlx, MLX_KEY_LEFT_SHIFT)
        && !mlx_is_key_down(game->mlx, MLX_KEY_RIGHT_SHIFT)))
    {
        dx *= 0.5f;
        dy *= 0.5f;
    }
    // Met à jour la position du joueur
    game->player->x += (dx / 33.f * SIZE) / vector_length;
    game->player->y += (dy / 33.f * SIZE) / vector_length;
}

// Gère les entrées clavier pour déplacer le joueur et tourner la caméra
void key_hook(t_game *game)
{
    // Déplacement avant
    if (mlx_is_key_down(game->mlx, UP1))
        make_move(game, game->player->dx, game->player->dy);

    // Déplacement arrière
    if (mlx_is_key_down(game->mlx, DOWN1))
        make_move(game, -1.f * game->player->dx, -1.f * game->player->dy);

    // Déplacement latéral gauche
    if (mlx_is_key_down(game->mlx, LEFT1))
        make_move(game, game->player->dy, -1.f * game->player->dx);

    // Déplacement latéral droit
    if (mlx_is_key_down(game->mlx, RIGHT1))
        make_move(game, -1.f * game->player->dy, game->player->dx);

    // Rotation à gauche
    if (mlx_is_key_down(game->mlx, LEFT2))
    {
        game->player->a -= RADIANS;
        if (game->player->a < 0)
            game->player->a += 2.f * PI;
        game->player->dx = cos(game->player->a) * 4.f;
        game->player->dy = sin(game->player->a) * 4.f;
    }

    // Rotation à droite
    if (mlx_is_key_down(game->mlx, RIGHT2))
    {
        game->player->a += RADIANS;
        if (game->player->a > 2.f * PI)
            game->player->a -= 2.f * PI;
        game->player->dx = cos(game->player->a) * 4.f;
        game->player->dy = sin(game->player->a) * 4.f;
    }

    // Mode débogage : met à jour la position du sprite du joueur

    // Redessine la carte et les rayons
    if (mlx_is_key_down(game->mlx, UP1)
        || mlx_is_key_down(game->mlx, DOWN1)
        || mlx_is_key_down(game->mlx, LEFT1)
        || mlx_is_key_down(game->mlx, RIGHT1)
        || mlx_is_key_down(game->mlx, LEFT2)
        || mlx_is_key_down(game->mlx, RIGHT2))
    {
        draw_map(game, 0);
        draw_ray(game);
    }
}

// Convertit une couleur RGB en format hexadécimal 32 bits

void init_game(t_game *game)
{
    // static char *str = "./textures/masterwu.png";
    game->player = ft_calloc(sizeof(t_player), 1);
    if (game->info->pos == 'E')
		game->player->a = 0*RADIANS;
    if (game->info->pos == 'S')
        game->player->a = 90*RADIANS;
    if (game->info->pos == 'W')
        game->player->a = 180*RADIANS;
    if (game->info->pos == 'N')
		game->player->a = 270*RADIANS;
    // printf("\n\nNO:[%s]\nSU:[%s]\nWE:[%s]\nEA:[%s]\n", game->info->north, game->info->south, game->info->west, game->info->east);
    game->textures = ft_calloc(sizeof(t_textures), 1);
    game->textures->north = mlx_load_png(game->info->north);
    game->textures->south = mlx_load_png(game->info->south);
    game->textures->east = mlx_load_png(game->info->east);
    game->textures->west = mlx_load_png(game->info->west);
    game->player->dx = cos(game->player->a) * 4.f;
    game->player->dy = sin(game->player->a) * 4.f;
}

// Initialise le jeu et lance la boucle principale
void ft_game(t_game *game)
{
    mlx_image_t     *background;

    // Initialise la taille de la carte
    game->mapsize[1] = ft_tablen(game->tab);
    game->mapsize[0] = ft_strlen(game->tab[0]);

    init_game(game);

    // Configure MLX42
    mlx_set_setting(MLX_STRETCH_IMAGE, true);
    game->mlx = mlx_init(WIDTH, HEIGHT, "cub3D", true);
    if (!game->mlx)
        error_msg(MLX_CANNOT_CREATE, NULL);

    game->sprite = ft_calloc(sizeof(t_sprite), 1);
    game->sprite->north = mlx_texture_to_image(game->mlx, game->textures->north);
    game->sprite->south = mlx_texture_to_image(game->mlx, game->textures->south);
    game->sprite->east = mlx_texture_to_image(game->mlx, game->textures->east);
    game->sprite->west = mlx_texture_to_image(game->mlx, game->textures->west);
    mlx_resize_image(game->sprite->north, SIZE, SIZE);
    mlx_resize_image(game->sprite->south, SIZE, SIZE);
    mlx_resize_image(game->sprite->east, SIZE, SIZE);
    mlx_resize_image(game->sprite->west, SIZE, SIZE);

    // Crée un fond d'écran avec les couleurs du plafond et du sol
    background = mlx_new_image(game->mlx, 1, 2);
    mlx_put_pixel(background, 0, 0, rgb_to_hex32(game->info->info[1])); // Couleur du plafond
    mlx_put_pixel(background, 0, 1, rgb_to_hex32(game->info->info[0])); // Couleur du sol
    mlx_resize_image(background, WIDTH, HEIGHT);
    mlx_image_to_window(game->mlx, background, 0 , 0);

    // Crée une image pour l'écran
    game->screen = mlx_new_image(game->mlx, WIDTH, HEIGHT);

    // Affiche les textures et l'écran
    mlx_image_to_window(game->mlx, game->screen, 0, 0);

    // Dessine la carte et les rayons
    draw_map(game, 1);
    draw_ray(game);

    // Configure la boucle principale
    mlx_loop_hook(game->mlx, (void (*))key_hook, (void *)game);
    mlx_loop(game->mlx);

    // Termine MLX42
    mlx_terminate(game->mlx);
}
