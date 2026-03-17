NAME        = Gomoku
BUILD_DIR   = build

all: $(NAME)

$(NAME):
	@cmake -B $(BUILD_DIR) -S .
	@cmake --build $(BUILD_DIR) --parallel

clean:
	@echo "Suppression du dossier $(BUILD_DIR)..."
	@rm -rf $(BUILD_DIR)

run: all
	@cd $(BUILD_DIR) && ./$(NAME)

fclean: clean

re: fclean all

.PHONY: all clean fclean re
