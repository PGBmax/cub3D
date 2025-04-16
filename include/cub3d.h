/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:19:25 by pboucher          #+#    #+#             */
/*   Updated: 2025/04/16 14:06:51 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

/*	Includes	*/
#include "../MLX42/include/MLX42/MLX42.h"
#include <../libft/libft.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <math.h>

/*	Macros	*/
# define PI M_PI
# define RADIANS 0.0174533f
# define WIDTH 1024
# define HEIGHT 1024
# define SIZE 128
# ifndef DEBUG
#  define DEBUG 0
# endif


# define UP1 MLX_KEY_W
# define DOWN1 MLX_KEY_S
# define LEFT1 MLX_KEY_A
# define RIGHT1 MLX_KEY_D
# define UP2 MLX_KEY_UP
# define DOWN2 MLX_KEY_DOWN
# define LEFT2 MLX_KEY_LEFT
# define RIGHT2 MLX_KEY_RIGHT


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
# define NOT_CLOSE "%s : %s : The Map is not hermetic\n"
# define MLX_CANNOT_CREATE "%s : MLX cannot be created\n"
# define FAIL_LOAD "%s : MLX has failed to load png\n"
# define MAP_INFO_FAIL "%s : %s : Map info not properly set\n"

/*  Structs  */

typedef struct s_textures
{
	mlx_texture_t	*wall[4];
}					t_textures;

typedef struct s_sprite
{
	mlx_image_t	*player;
	mlx_image_t	*wall;
	mlx_image_t	*ground;
}				t_sprite;

typedef struct s_ray
{
	float	DirX;
	float	DirY;
	int		mapX;
	int		mapY;
	float	deltaDistX;
	float	deltaDistY;
	int		stepX;
	int		stepY;
	float	sideDistX;
	float	sideDistY;
	float	hitX;
	float	hitY;
}	t_ray;


typedef struct s_info
{
	char	*north;
	char	*south;
	char	*east;
	char	*west;
	char	*floor;
	char	*ceiling;
	int		fc[6];
	int 	start_index;
}	t_info;

typedef	struct s_player
{
	float	x;
	float	y;
	float dx;
	float dy;
	float a;
}	t_player;

typedef struct s_game
{
	t_player	*player;
	t_map		*map;
	t_info		*info;
	char		**tab;
	mlx_image_t	*screen;
	t_sprite	sprite;
	mlx_t		*mlx;
	int			mapsize[2];
}	t_game;

/*	Prototypes	*/
//	game.c
void	ft_game(t_game *game);
//	get_game.c
void	set_tgame(t_game *info);
t_game	*get_tgame(void);
//	output.c
void	error_msg(char *str, char *detail);
//	parsing.c
bool	is_valid(char *path);
t_map	*get_map_as_list(char *argv);
char	**get_map_as_tab(t_map *lstmap, int index);
int		is_map_valid(char **map);
int		line_length(char *str);
char	*void_changer(char *str);
int		parse_tab(char **tab);
int		get_map_info(t_game *game, int i);

#endif