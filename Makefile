# === Project name ===
NAME = philo

# === Directories ===
SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = includes

# === Source Subdirectories ===
SRC_SUBDIRS = \
	$(SRC_DIR)/config \
	$(SRC_DIR)/core \
	$(SRC_DIR)/utils

# === Source files ===
SRCS = \
	$(SRC_DIR)/main.c \
	$(foreach dir, $(SRC_SUBDIRS), $(wildcard $(dir)/*.c))

# === Object files ===
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# === Compiler flags ===
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
INCLUDES = -I$(INCLUDE_DIR)

# === Default rule ===
all: $(NAME)

# === Compile rule for .o files ===
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# === Linking rule ===
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

# === Clean object files ===
clean:
	@echo "Cleaning object files..."
	@rm -rf $(OBJ_DIR)

# === Clean everything ===
fclean: clean
	@echo "Cleaning $(NAME)..."
	@rm -f $(NAME)

# === Rebuild everything ===
re: fclean all

.PHONY: all clean fclean re