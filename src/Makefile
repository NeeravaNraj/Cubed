CC:=gcc
CFLAGS:=-c -g -m64 -I inc
SRC_FILES = $(shell find -type f -name "*.c")
SRC_DIRS = $(shell find . -type d)
OBJ_FILES = $(abspath $(addprefix $(BUILD_DIR)/, $(SRC_FILES:.c=.o)))
BUILD_DIR=$(abspath ../build/o)
DIR_GUARD = mkdir -p $(@D)
OUTDIRS = $(addprefix $(BUILD_DIR)/, $(SRC_DIRS))
H_FILES = $(shell find -type f -name "*.h")


compile: $(OBJ_FILES)
$(BUILD_DIR)/%.o: %.c $(H_FILES)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $< -o $@
