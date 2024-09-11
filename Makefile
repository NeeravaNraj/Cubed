BUILD_DIR = build
SRC_DIR = src
LD = gcc
OBJ_FILES = $(shell find $(BUILD_DIR)/o -type f -name "*.o")
BIN_DIR = $(BUILD_DIR)/bin

.PHONY: all link compile clean run deps

all: link

link: compile
	@echo -----------
	@echo Linking....
	@echo -----------
	$(LD) -fstack-protector-all -o $(BIN_DIR)/main.bin $(OBJ_FILES) -L ./libs -l:libraylib.a -lm
	@echo Done.

compile: $(BUILD_DIR)
	@echo -------------
	@echo Compiling....
	@echo -------------
	@$(MAKE) --no-print-directory -C ./src

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BIN_DIR)


clean:
	rm -rf build
