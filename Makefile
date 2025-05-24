# Compilation
CC					=	cc
CFLAGS				=	-MP -MMD -Wall -Wextra -Werror -g -I $(INCLUDES)

LIBMLX = ./MLX42
LIBS = $(LIBMLX)/build/libmlx42.a -ldl -lglfw -pthread -lm

# Names
NAME				=	cub3D

# Sources & Includes
SRCS	= 	0_MANDATORY/main.c \
			0_MANDATORY/output.c \
			0_MANDATORY/game.c \
			0_MANDATORY/hook.c \
			0_MANDATORY/move.c \
			0_MANDATORY/draw.c \
			0_MANDATORY/raycast.c \
			0_MANDATORY/game_init.c \
			0_MANDATORY/get_game.c \
			0_MANDATORY/parsing.c \
			0_MANDATORY/map_parsing.c \
			0_MANDATORY/map_info.c \
			0_MANDATORY/free.c

SRCSBONUS	= 	1_BONUS/main.c \
				1_BONUS/output.c \
				1_BONUS/game.c \
				1_BONUS/hook.c \
				1_BONUS/move.c \
				1_BONUS/wall.c \
				1_BONUS/raycast.c \
				1_BONUS/game_init.c \
				1_BONUS/get_game.c \
				1_BONUS/parsing.c \
				1_BONUS/map_parsing.c \
				1_BONUS/map_info.c \
				1_BONUS/free.c

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
	# @rm -rf ./MLX42
	$(ALL_CLEAN)

fclean :
	make fclean -C ./3_LIBFT
	@rm -f $(NAME)
	@rm -rf $(OBJ_FOLDER)
	@rm -rf $(OBJ_FOLDER_BONUS)
	# @rm -rf ./MLX42	
	$(ALL_FCLEAN)

re : fclean all

bonus : mlx libft $(OBJSBONUS)
	@$(CC) $(CFLAGS) $(INCLUDES) $(OBJSBONUS) $(LIB) $(LIBS) -o $(NAME)
	$(EXE_DONE_BONUS)

$(OBJ_FOLDER_BONUS)/%.o: 1_BONUS/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: all clean fclean re libft

-include $(DEPS)
-include $(DEPSBONUS)
