CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2 -I./src
LDFLAGS = -lm

SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

SOURCES = $(SRC_DIR)/log.c $(SRC_DIR)/angle.c $(SRC_DIR)/state_machine.c \
          $(SRC_DIR)/velocity.c $(SRC_DIR)/rpe.c $(SRC_DIR)/form.c $(SRC_DIR)/rep_engine.c

TEST_SOURCES = $(TEST_DIR)/test_log.c $(TEST_DIR)/test_angle.c $(TEST_DIR)/test_state_machine.c \
               $(TEST_DIR)/test_velocity.c $(TEST_DIR)/test_rpe.c $(TEST_DIR)/test_form.c \
               $(TEST_DIR)/test_rep_engine.c

OBJECTS = $(SOURCES:.c=.o)
TEST_OBJECTS = $(TEST_SOURCES:.c=.o)

TARGET = librepengine.a
TEST_TARGET = repengine_tests
TEST_BINARIES = $(TEST_SOURCES:.c=)

.PHONY: all clean test lib

all: lib test

lib: $(TARGET)

$(TARGET): $(OBJECTS)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_BINARIES)
	@for t in $(TEST_BINARIES); do ./$$t; done

tests/%: $(TEST_DIR)/%.o $(OBJECTS) | $(BUILD_DIR)
	$(CC) $^ -o $@ $(LDFLAGS)

$(TEST_DIR)/%.o: $(TEST_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I./tests -I./src -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -f $(OBJECTS) $(TEST_OBJECTS) $(TARGET) $(TEST_TARGET)
	rm -rf $(BUILD_DIR)
