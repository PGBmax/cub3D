/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:19:25 by pboucher          #+#    #+#             */
/*   Updated: 2025/06/15 17:32:05 by pboucher         ###   ########.fr       */
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
# define WIDTH 960
# define HEIGHT 540
# define S_BOX 512
# define FOV 0.9f
# define MOVESPD 0.05f
# define ROTSPD 0.05f
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
	mlx_texture_t	*door;
	mlx_texture_t	*mapground;
	mlx_texture_t	*mapwall;
	mlx_texture_t	*mapplayer;
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
	mlx_image_t	*door;
	mlx_image_t	*mapground;
	mlx_image_t	*mapwall;
	mlx_image_t	*mapplayer;
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
	float dim;

	int	x;

	float wallX;
	float step;
	float texPos;
	int texX;
	int texY;

	float rayDirX0;
	float rayDirY0;
	float rayDirX1;
	float rayDirY1;
	int p;
	float posZ;
	float rowDistance;
	float floorStepX;
	float floorStepY;
	float floorX;
	float floorY;
	int cellX;
	int cellY;
	int tx;
	int ty;

}	t_ray;

typedef struct s_matrix
{
	uint32_t	**north;
	uint32_t	**south;
	uint32_t	**west;
	uint32_t	**east;
	uint32_t	**floor;
	uint32_t	**ceilling;
	uint32_t	**door;
}				t_matrix;

typedef struct s_info
{
	char	*north;
	char	*south;
	char	*east;
	char	*west;
	char	*floor;
	char	*ceiling;
	char	*door;
	int 	start_index;
	int		info[2][3];
	char	pos;
}	t_info;

typedef int	t_byte;

typedef	struct s_color
{
	t_byte			red;
	t_byte			green;
	t_byte			blue;
	t_byte			opacity;
	unsigned int	color;
}	t_color;

typedef	struct s_player
{
	float	x;
	float	y;
}	t_player;

typedef	struct s_pos
{
	float x;
	float y;
} 				t_pos;

typedef struct s_game
{
	t_player	*player;
	t_pos		*pos;
	t_map		*map;
	t_info		*info;
	char		**tab;
	mlx_image_t	*screen;
	t_sprite	*sprite;
	t_textures	*textures;
	t_matrix	*matrix;
	t_ray		*ray;
	mlx_t		*mlx;
	int			mapsize[2];
	int			paused;
}	t_game;

/*	Prototypes	*/
/*			0_PARSING			*/
//	free.c
void		free_matrix(uint32_t **matrix);
void		free_texture(mlx_texture_t *texture);
void		free_game2(t_game *game);
void		free_game(t_game *game);
//	get_map.c
char		**longest_line(char **tab, int i);
t_map		*get_map_as_list(char *argv);
char		**alloc_map(t_map *lstmap, t_map **tmp);
char		**get_map_as_tab(t_map *lstmap, int index);
//	load.c
int			assign_frame(t_game *game, char **modif2, char **path, int i);
int			load_frames(t_game *game);
int			load_image(t_game *game);
int			load_matrix(t_game *game);
//	map_info.c
int			check_info(t_game *game);
char		*dup_cutendl(char *src, char *cpy);
void		get_str_info(t_map *tmp, t_game *game, int *i);
int			get_map_info(t_game *game, int i);
int			line_length(char *str);
//	map_parsing.c
int			check_hole(char **tab, int i, int j);
int			edge_parsing(char **tab);
int			parse_tab(t_game *game, int i, int j, int _bool);
//	output.c
void		error_msg(char *str, char *detail);
//	parsing.c
bool		is_valid(char *path);
char		*void_changer(char *str);
int			mapverif(char **map, int i, int j);
/*			1_RAYCAST			*/
//	floor_ceil.c
void		calculate_floor_ceilling(t_game *game, int y);
void		draw_floor_ceilling(t_game *game);
//	maths.c
t_pos		adding_pos(t_pos a, t_pos b);
float		calc_len(t_pos pos);
float		calc_dist(t_pos a, t_pos b);
int			in_circle(t_pos player, t_pos center, float radius);
t_pos		newpos(float x, float y);
//	raycast.c
void	draw_ray(t_game *game);
void	detect_door(t_game *game);
//	wall.c
uint32_t	get_color(mlx_image_t *img, int x, int y);
uint32_t	choose_wall(t_game *game, int texX, int texY);
void		print_wall(t_game *game, int x);
void		draw_wall(t_game *game, int x);
/*			2_GAME				*/
//	game_init.c
mlx_image_t	*convert_and_resize(mlx_texture_t *tex);
uint32_t	**convert_into_matrix(mlx_image_t *img);
int			game_init(t_game *game);
//	game.c
void		draw_map(t_game *game);
void		refresh_minimap(t_game *game);
void		refresh(t_game *game);
int			ft_game(t_game *game);
//	get_game.c
void		set_tgame(t_game *info);
t_game		*get_tgame(void);
//	hook.c
void		key_hook(t_game *game);
void		cursor_hook(t_game *game);
void		game_pause(mlx_key_data_t key_data, t_game *game);
//	minimap.c
t_color		set_color(t_byte r, t_byte g, t_byte b, t_byte a);
t_color		pick_color(t_game *game, t_pos loop);
void		draw_minimap(t_game *game, t_color color, t_pos pos);
uint32_t	minimap(t_game *game, t_pos pos);
//	move.c
void		move_player(t_game *game, float x, float y);
void		rotate_cam(t_game *game, float rotSpeed);
#endif
