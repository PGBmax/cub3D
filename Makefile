# Compilation
CC					=	cc
CFLAGS				=	-MP -MMD -Wall -Wextra -Werror -g -I $(INCLUDES)

LIBMLX = ./MLX42
LIBS = $(LIBMLX)/build/libmlx42.a -ldl -lglfw -pthread -lm

# Names
NAME				=	cub3D

# Sources & Includes
SRCS				= 	srcs/main.c \
						srcs/output.c \
						srcs/game.c \
						srcs/get_game.c \
						srcs/parsing.c \
						srcs/map_parsing.c \
						srcs/map_info.c \
						srcs/free.c

OBJ_FOLDER			=	objs
LIB           		=	libft/libft.a
INCLUDES 			= 	-Iinclude/ -I$(LIBMLX)/include

# Objects
OBJS				=	$(patsubst srcs/%, $(OBJ_FOLDER)/%, $(SRCS:.c=.o))
DEPS				=	$(patsubst srcs/%, $(OBJ_FOLDER)/%, $(SRCS:.c=.d))

# Custom Makefile Flags
MAKEFLAGS			+=	--no-print-directory --silent

# Custom Colors
PURPLE				=	\033[1;35m
LIGHT_GREEN			=	\033[1;32m
RESET				=	\033[0m

# Custom messages
EXE_DONE			=	@echo "$(PURPLE)🎉 $(NAME) compiled! 🎉$(RESET)\n"
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

$(OBJ_FOLDER)/%.o: srcs/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

libft : 
	@if [ ! -f $(LIB) ]; then \
        make -C ./libft; \
	fi

clean :
	make clean -C ./libft
	@rm -rf $(OBJ_FOLDER)
	$(ALL_CLEAN)

fclean :
	make fclean -C ./libft
	@rm -f $(NAME)
	@rm -rf $(OBJ_FOLDER)
	$(ALL_FCLEAN)

re : fclean all

.PHONY: all clean fclean re libft

-include $(DEPS)