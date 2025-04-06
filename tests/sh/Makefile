CC = gcc
CFLAGS = -I../unity -I../../include -c
BUILD_DIR = build
TARGET = $(BUILD_DIR)/tests
CONFIG_SRC = ../../src/config.c
CLASIF_SRC = ../../src/clasif.c
THREADS_SRC = ../../src/threads.c
SIGNALS_SRC = ../../src/signals.c
MAIN_SRC = ../../src/main.c
TEST_SRC_DIR = .


CONFIG_OBJ = $(BUILD_DIR)/config.o
CLASIF_OBJ = $(BUILD_DIR)/clasif.o
THREADS_OBJ = $(BUILD_DIR)/threads.o
SIGNALS_OBJ = $(BUILD_DIR)/signals.o
MAIN_OBJ = $(BUILD_DIR)/main.o

OBJ = $(CONFIG_OBJ) $(THREADS_OBJ) $(SIGNALS_OBJ) $(MAIN_OBJ) $(CLASIF_OBJ) 

all: $(TARGET)

$(CONFIG_OBJ): $(CONFIG_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(MAIN_OBJ): $(MAIN_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(SIGNALS_OBJ): $(SIGNALS_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(THREADS_OBJ): $(THREADS_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(CLASIF_OBJ): $(CLASIF_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ -I../../include -pthread

run: $(TARGET)
	mkdir -p "../../logs_sh"
	cd ../../ && make
	sleep 2
	chmod +x *.sh
	./launch.sh > ../../logs_sh/logs_sh_test.log

clean:
	rm -rf $(BUILD_DIR)