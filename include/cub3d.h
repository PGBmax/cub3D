/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:42:54 by pboucher          #+#    #+#             */
/*   Updated: 2025/04/08 17:21:03 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef CUB3D_H
# define CUB3D_H

/*	Includes	*/
#include "../MLX42/include/MLX42/MLX42.h"
#include <../libft/libft.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>

/*	Macros	*/
# define WIDTH 800
# define HEIGHT 600


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
# define NOT_CLOSE "%s : %s : Map has a hole inside\n"
# define MLX_CANNOT_CREATE "%s : MLX cannot be created\n"

/*  Structs  */

typedef struct s_textures
{
	mlx_texture_t	*ground;
	mlx_texture_t	*wall;
	mlx_texture_t	*sky;
	mlx_texture_t	*npc;
}					t_textures;

typedef struct s_sprite
{
	mlx_image_t	*ground;
	mlx_image_t	*wall;
	mlx_image_t	*sky;
	mlx_image_t	*npc;
}				t_sprite;


typedef struct s_info
{
	char	*north;
	char	*south;
	char	*east;
	char	*west;
	char	*floor;
	char	*ceiling;
}	t_info;

typedef struct s_game
{
	t_map			*map;
	t_info			*info;
	char			**tab;
	t_textures	*textures;
	t_sprite	*sprite;
	mlx_t		*mlx;
}	t_game;

/*	Prototypes	*/
//	get_game.c
void	set_tgame(t_game *info);
t_game	*get_tgame(void);
//	output.c
void	error_msg(char *str, char *detail);
//	parsing.c
bool	is_valid(char *path);
int		is_map_valid(char **map);
int		line_length(char *str);
char	*void_changer(char *str);
//	map_parsing.h
t_map	*get_map_as_list(char *argv);
char	**get_map_as_tab(t_map *lstmap);
int		check_hole(char **tab, int i, int j);
int		edge_parsing(char **tab);
int		parse_tab(char **tab);

#endif