CC = gcc

TARGET = pico
BUILD_DIR = build
BUILD_DEBUG_DIR = $(BUILD_DIR)/debug
BUILD_RELEASE_DIR = $(BUILD_DIR)/release
SRC_DIR = src
SRCS := $(wildcard $(SRC_DIR)/*.c)

ifeq ($(strip $(SRCS)),)
$(error No C sources found in $(SRC_DIR))
endif

CFLAGS_COMMON = -Wall -Wextra -Werror -std=c11
CFLAGS_DEBUG   = $(CFLAGS_COMMON) -g3 -O0 -fsanitize=address,undefined -fno-omit-frame-pointer
CFLAGS_RELEASE = $(CFLAGS_COMMON) -O2 -DNDEBUG -march=native -flto

DEPFLAGS = -MMD -MP

all: debug

debug: CFLAGS = $(CFLAGS_DEBUG)
debug: $(BUILD_DEBUG_DIR)/$(TARGET)

release: CFLAGS = $(CFLAGS_RELEASE)
release: $(BUILD_RELEASE_DIR)/$(TARGET)

OBJS_DEBUG := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DEBUG_DIR)/%.o,$(SRCS))
OBJS_RELEASE := $(patsubst $(SRC_DIR)/%.c,$(BUILD_RELEASE_DIR)/%.o,$(SRCS))

DEPS := $(OBJS_DEBUG:.o=.d) $(OBJS_RELEASE:.o=.d)
-include $(DEPS)

$(BUILD_DEBUG_DIR)/$(TARGET): $(OBJS_DEBUG) | $(BUILD_DEBUG_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_RELEASE_DIR)/$(TARGET): $(OBJS_RELEASE) | $(BUILD_RELEASE_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DEBUG_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DEBUG_DIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(BUILD_RELEASE_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_RELEASE_DIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(BUILD_DEBUG_DIR) $(BUILD_RELEASE_DIR):
	mkdir -p $@

run: debug
	$(BUILD_DEBUG_DIR)/$(TARGET)

run-release: release
	$(BUILD_RELEASE_DIR)/$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all debug release run run-release clean
