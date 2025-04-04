/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maregnie <maregnie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:42:54 by pboucher          #+#    #+#             */
/*   Updated: 2025/04/04 12:50:08 by maregnie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# define CUB3D_H

/*	Includes	*/
#include "../MLX42/include/MLX42/MLX42.h"
#include <../libft/libft.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>

/*	Colors	*/
# define RST "\e[0m"
# define RED "\e[0;31m"
# define GRN "\e[0;32m"
# define YEL "\e[0;33m"
# define BLU "\e[0;34m"
# define MAG "\e[0;35m"
# define CYN "\e[0;36m"
# define WHT "\e[0;37m"

/*	Errors	*/
# define PROG_NAME "cub3D"
# define CORRECT_USAGE "%s : Correct usage: ./cub3D <map.cub>\n"
# define INVALID_TERM "%s : %s: Invalid extension\n"
# define MALLOC_ERROR "%s : Malloc failed\n"
# define INVALID_NAME "%s : Invalid Map Name\n"
# define CANNOT_OPEN "%s : %s : File doesn't exist\n"

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

typedef struct s_game
{
	t_map			*map;
	char			**tab;
	t_textures	*textures;
	t_sprite	*sprite;
	mlx_t		*mlx;
}	t_game;

/*	Prototypes	*/
//	output.c
void	error_msg(char *str, char *detail);
//	parsing.c
bool	is_valid(char *path);
t_map	*get_map_as_list(char *argv);
char	**get_map_as_tab(t_map *lstmap);
int		is_map_valid(char **map);
int		line_length(char *str);
char	*void_changer(char *str);

#endif