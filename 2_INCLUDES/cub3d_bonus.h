/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:19:25 by pboucher          #+#    #+#             */
/*   Updated: 2025/05/24 17:54:30 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

/*	Includes	*/
#include "../MLX42/include/MLX42/MLX42.h"
#include <../3_LIBFT/libft.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <math.h>

/*	Macros	DONT TOUCH	*/
# define PI M_PI
# define RADIANS 0.0174533f
# define WIDTH 800
# define HEIGHT 450
# define S_WIDTH 256
# define S_HEIGHT 256
# define FOV 0.9f
# define MOVESPD 0.035f
# define ROTSPD 0.035f
# define RGBLEN 3
# define UP MLX_KEY_W
# define DOWN MLX_KEY_S
# define LEFT MLX_KEY_A
# define RIGHT MLX_KEY_D
# define LEFT_R MLX_KEY_LEFT
# define RIGHT_R MLX_KEY_RIGHT

/*	Colors	*/
# define RST "\e[0m"
# define RED "\e[1;91m"
# define GRN "\e[1;92m"
# define YEL "\e[1;93m"
# define BLU "\e[1;94m"
# define MAG "\e[1;95m"
# define CYN "\e[1;96m"
# define WHT "\e[1;97m"

/*	Errors	*/
# define PROG_NAME "cub3D"
# define CORRECT_USAGE "%s : Correct usage: ./cub3D <map.cub>\n"
# define INVALID_TERM "%s : %s: Invalid extension\n"
# define MALLOC_ERROR "%s : Malloc failed\n"
# define INVALID_NAME "%s : Invalid Map Name\n"
# define CANNOT_OPEN "%s : %s : File doesn't exist\n"
# define NOT_CLOSE "%s : %s : The Map is not hermetic or player pos not set properly\n"
# define MLX_CANNOT_CREATE "%s : MLX cannot be created\n"
# define FAIL_LOAD "%s : MLX has failed to load png\n"
# define MAP_INFO_FAIL "%s : %s : Map info not properly set\n"

/*  Structs  */

typedef struct s_textures
{
	mlx_texture_t	*icon;
	mlx_texture_t	*north;
	mlx_texture_t	*south;
	mlx_texture_t	*west;
	mlx_texture_t	*east;
	mlx_texture_t	*floor;
	mlx_texture_t	*ceilling;
	mlx_texture_t	*frames[44];
	mlx_texture_t	*pause;
}					t_textures;

typedef struct s_sprite
{
	mlx_image_t	*north;
	mlx_image_t	*south;
	mlx_image_t	*west;
	mlx_image_t	*east;
	mlx_image_t	*floor;
	mlx_image_t	*ceilling;
	mlx_image_t	*frames[44];
	mlx_image_t	*pause;
}				t_sprite;

typedef struct s_ray
{
	float posX;
	float posY;
	float dirX;
	float dirY;
	float planeX;
	float planeY;

	float cameraX;
	float rayDirX;
	float rayDirY;

	int mapX;
	int mapY;

	float sideDistX;
	float sideDistY;

	float deltaDistX;
	float deltaDistY;
	float perpWallDist;

	int stepX;
	int stepY;

	int hit;
	int side;

	int lineH;
	int drawStart;
	int drawEnd;

	float oldDirX;
	float oldPlaneX;
	
	float moveSpeed;
	float rotSpeed;
	
	uint32_t color;

	float wallX;
	float step;
	float texPos;
	int texX;
	int texY;

}	t_ray;

typedef struct s_info
{
	char	*north;
	char	*south;
	char	*east;
	char	*west;
	char	*floor;
	char	*ceiling;
	int 	start_index;
	int		info[2][3];
	char	pos;
}	t_info;

typedef	struct s_player
{
	float	x;
	float	y;
}	t_player;

typedef struct s_game
{
	t_player	*player;
	t_map		*map;
	t_info		*info;
	char		**tab;
	mlx_image_t	*screen;
	t_sprite	*sprite;
	t_textures	*textures;
	t_ray		*r;
	mlx_t		*mlx;
	int			mapsize[2];
	int			paused;
}	t_game;

/*	Prototypes	*/
//	game.c
int	ft_game(t_game *game);
uint32_t    rgb_to_hex32(int *rgb);
void	refresh(t_game *game);
//	wall.c
uint32_t get_color(mlx_image_t *img, int x, int y);
void	draw_wall(t_game *game, int x);
//	get_game.c
void	set_tgame(t_game *info);
t_game	*get_tgame(void);
//	output.c
void	error_msg(char *str, char *detail);
//	parsing.c
bool	is_valid(char *path);
t_map	*get_map_as_list(char *argv);
char	**get_map_as_tab(t_map *lstmap, int index);
int		line_length(char *str);
char	*void_changer(char *str);
int		parse_tab(t_game *game, int i, int j, int _bool);
int		get_map_info(t_game *game, int i);
int		place_textures(t_game *game);
int		convert_textures(t_game *game);
void	free_game(t_game *game);
//	game_init.c
int		game_init(t_game *game);
//	hook.c
void    key_hook(t_game *game);
void    cursor_hook(t_game *game);
void	game_pause(mlx_key_data_t key_data, t_game *game);
//	move.c
void    move_player(t_game *game, float x, float y);
void    rotate_cam(t_game *game, float rotSpeed);
//	raycast.c
void    draw_ray(t_game *game);

#endif