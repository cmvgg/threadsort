CC = gcc
CFLAGS = -I../unity -I../../include -c
BUILD_DIR = build
TARGET = $(BUILD_DIR)/unit_tests
UNITY_SRC = ../unity.c
CONFIG_SRC = ../../src/config.c
CLASIF_SRC = ../../src/clasif.c
TEST_RUNNER_SRC = test_runner.c
TEST_SRC_DIR = .

TEST_SOURCES = $(filter-out $(TEST_RUNNER_SRC), $(wildcard $(TEST_SRC_DIR)/test_*.c))

UNITY_OBJ = $(BUILD_DIR)/unity.o
CONFIG_OBJ = $(BUILD_DIR)/config.o
CLASIF_OBJ = $(CLASIF_DIR)/clasif.o
TEST_RUNNER_OBJ = $(BUILD_DIR)/$(TEST_RUNNER_SRC:.c=.o)
TEST_OBJS = $(patsubst $(TEST_SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(TEST_SOURCES))

OBJ = $(UNITY_OBJ) $(CONFIG_OBJ) $(CLASIF_OBJ) $(TEST_RUNNER_OBJ) $(TEST_OBJS)

all: $(TARGET)

$(UNITY_OBJ): $(UNITY_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(CONFIG_OBJ): $(CONFIG_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(CLASIF_OBJ): $(CLASIF_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(TEST_RUNNER_OBJ): $(TEST_RUNNER_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: $(TEST_SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ -I../../include -pthread

run: $(TARGET)
	./$(TARGET)  >../../logs_unit/logs_unit_test.log

clean:
	rm -rf $(BUILD_DIR)