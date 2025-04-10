/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:19:25 by pboucher          #+#    #+#             */
/*   Updated: 2025/04/10 15:28:03 by pboucher         ###   ########.fr       */
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
# define WIDTH 800
# define HEIGHT 600
# define SIZE 64
# define FOV 90


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
	mlx_texture_t	*fc;
	mlx_texture_t	*wall[4];
}					t_textures;

typedef struct s_sprite
{
	mlx_image_t	*fc;
	mlx_image_t	*wall[4];
}				t_sprite;


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

typedef struct s_game
{
	t_map		*map;
	t_info		*info;
	char		**tab;
	t_textures	textures;
	t_sprite	sprite;
	mlx_t		*mlx;
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