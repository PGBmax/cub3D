# Compilation
CC					=	cc
CFLAGS				=	-MP -MMD -Wall -Wextra -Werror -g -I $(INCLUDES)

LIBMLX = ./MLX42
LIBS = $(LIBMLX)/build/libmlx42.a -ldl -lglfw -pthread -lm

# Names
NAME				=	cub3D
NAMEBONUS			=	cub3D_bonus

# Sources & Includes
SRCS		= 	0_MANDATORY/0_PARSING/free.c \
				0_MANDATORY/0_PARSING/get_map.c \
				0_MANDATORY/0_PARSING/map_info.c \
				0_MANDATORY/0_PARSING/map_parsing.c \
				0_MANDATORY/0_PARSING/output.c \
				0_MANDATORY/0_PARSING/parsing.c \
				0_MANDATORY/1_RAYCAST/draw.c \
				0_MANDATORY/1_RAYCAST/raycast.c \
				0_MANDATORY/2_GAME/game_init.c \
				0_MANDATORY/2_GAME/game.c \
				0_MANDATORY/2_GAME/get_game.c \
				0_MANDATORY/2_GAME/hook.c \
				0_MANDATORY/2_GAME/move.c \
				0_MANDATORY/main.c

SRCSBONUS	= 	1_BONUS/0_PARSING/free.c \
				1_BONUS/0_PARSING/get_map.c \
				1_BONUS/0_PARSING/load.c \
				1_BONUS/0_PARSING/map_info.c \
				1_BONUS/0_PARSING/map_parsing.c \
				1_BONUS/0_PARSING/output.c \
				1_BONUS/0_PARSING/parsing.c \
				1_BONUS/1_RAYCAST/floor_ceil.c \
				1_BONUS/1_RAYCAST/maths.c \
				1_BONUS/1_RAYCAST/raycast.c \
				1_BONUS/1_RAYCAST/wall.c \
				1_BONUS/2_GAME/game_init.c \
				1_BONUS/2_GAME/game.c \
				1_BONUS/2_GAME/get_game.c \
				1_BONUS/2_GAME/hook.c \
				1_BONUS/2_GAME/minimap.c \
				1_BONUS/2_GAME/move.c \
				1_BONUS/main.c
				

OBJ_FOLDER			=	6_OBJECTS
OBJ_FOLDER_BONUS	=	7_OBJECTS_BONUS
LIB           		=	3_LIBFT/libft.a
INCLUDES 			= 	-I2_INCLUDES/ -I$(LIBMLX)/includes

# Objects
OBJS				=	$(patsubst 0_MANDATORY/%, $(OBJ_FOLDER)/%, $(SRCS:.c=.o))
DEPS				=	$(patsubst 0_MANDATORY/%, $(OBJ_FOLDER)/%, $(SRCS:.c=.d))
OBJSBONUS			=	$(patsubst 1_BONUS/%, $(OBJ_FOLDER_BONUS)/%, $(SRCSBONUS:.c=.o))
DEPSBONUS			=	$(patsubst 1_BONUS/%, $(OBJ_FOLDER_BONUS)/%, $(SRCSBONUS:.c=.d))

# Custom Makefile Flags
MAKEFLAGS			+=	--no-print-directory --silent

# Custom Colors
PURPLE				=	\033[1;35m
LIGHT_GREEN			=	\033[1;32m
RESET				=	\033[0m

# Custom messages
EXE_DONE			=	@echo "$(PURPLE)🎉 $(NAME) compiled! 🎉$(RESET)\n"
EXE_DONE_BONUS		=	@echo "$(PURPLE)🎉 $(NAME) (bonus) compiled! 🎉$(RESET)\n"
ALL_CLEAN			=	@echo "🧹$(LIGHT_GREEN) Project's objects cleaned! 🧹$(RESET)\n"
ALL_FCLEAN			=	@echo "🧹$(LIGHT_GREEN) Project's objects & Executables cleaned! 🧹$(RESET)\n"

# Rules

all : mlx libft $(NAME)

mlx :
	@if ls | grep -q "MLX42"; then \
		echo "\033[32;47;1m** MLX42 already exist **\033[1;m"; \
	else \
		git clone https://github.com/codam-coding-college/MLX42.git; \
		cmake ./MLX42 -B ./MLX42/build; \
		make -C ./MLX42/build --no-print-directory -j4; \
		make --directory ./MLX42/build; \
	fi

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIB) $(LIBS) -o $(NAME)
	$(EXE_DONE)

$(OBJ_FOLDER)/%.o: 0_MANDATORY/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

libft : 	
	@if [ ! -f $(LIB) ]; then \
        make -C ./3_LIBFT; \
	fi

clean :
	make clean -C ./3_LIBFT
	@rm -rf $(OBJ_FOLDER)
	@rm -rf $(OBJ_FOLDER_BONUS)
	@rm -rf ./MLX42
	$(ALL_CLEAN)

fclean :
	make fclean -C ./3_LIBFT
	@rm -f $(NAME)
	@rm -f $(NAMEBONUS)
	@rm -rf $(OBJ_FOLDER)
	@rm -rf $(OBJ_FOLDER_BONUS)
	@rm -rf ./MLX42	
	$(ALL_FCLEAN)

re : fclean all

bonus : mlx libft $(OBJSBONUS)
	@$(CC) $(CFLAGS) $(INCLUDES) $(OBJSBONUS) $(LIB) $(LIBS) -o $(NAMEBONUS)
	$(EXE_DONE_BONUS)

$(OBJ_FOLDER_BONUS)/%.o: 1_BONUS/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: all clean fclean re libft

-include $(DEPS)
-include $(DEPSBONUS)
