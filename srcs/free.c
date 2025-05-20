#include "cub3d.h"


void	free_game(t_game *game)
{
	if (game->info->north)
		free(game->info->north);
	if (game->info->south)
		free(game->info->south);
	if (game->info->east)
		free(game->info->east);
	if (game->info->west)
		free(game->info->west);
	free(game->info);
	if (game->r)
		free(game->r);
}