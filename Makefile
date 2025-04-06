CC = gcc
CFLAGS = -Wall -Werror -Wextra -Wpedantic -g3 -I./inc

SRC_DIR = src
OBJ_DIR = obj

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
EXE = order

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $(EXE)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

run_t: fclean $(EXE)
	./$(EXE) --file ./tests/config/valid_config.txt > exit1.txt

val: fclean $(EXE)
	valgrind -s --leak-check=full --show-leak-kinds=all ./$(EXE) --file ./tests/config/valid_config.txt

clean:
	rm -fr $(OBJ_DIR)

fclean: clean
	rm -fr $(EXE)

unit_test:
	docker-compose -f ./docker-compose.yml up --build --force-recreate -d

.PHONY: all run val clean fclean unit_test
