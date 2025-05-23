#include "cub3d.h"


// void	free_sprite(mlx_image_t	*sprite, t_game *game)
// {
// 	if (sprite)
// 		mlx_delete_image(game->mlx, sprite);
// }

void	free_texture(mlx_texture_t *texture)
{
	if (texture)
		mlx_delete_texture(texture);
}

void	free_game(t_game *game)
{
	free_texture(game->textures->east);
	free_texture(game->textures->west);
	free_texture(game->textures->north);
	free_texture(game->textures->south);
	free_texture(game->textures->pause);
	free_texture(game->textures->icon);
	if (game->sprite)	
		free(game->sprite);
	if (game->textures)
		free(game->textures);
	if (game->info)
		free(game->info);
	if (game->r)
		free(game->r);
}
