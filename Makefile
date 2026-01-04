##### BASIC MAKE TEMPLATE ####
# github.com/rfblock/basic-make
# A basic makefile template for C projects (MIT License)
# Version 0.1.1

# ==== CONFIG ====
# Target binary name
TARGET_EXEC := solve
# Enable this line to compile to a shared object (.so) instead of a binary
TARGET_LIBRARY := off
# If "debug", will add debug symbols, "release" removes symbols.
BUILD_MODE := debug
# (TODO) Zips executable, libraries, and resources
# "on" - Always package if $(TARGET_LIBRARY) is "on"
# "bin-only" - Only package if $(TARGET_LIBRARY) is also "off"
# "off" - Never package
PACKAGE_RELEASE := bin-only

# If directory is unused, do not blank out variable (leave as default value)
# Location of source code (.c)
SRC_DIR := src
# Location of compiled objects (.o/.d)
OBJ_DIR := obj
# Location of compiled target binary or shared object (.so)
BIN_DIR := bin
# Location of library/libraries source code
EXTERN_DIR := extern
# Location of included external headers (.h)
INCLUDE_DIR := include
# Location of dynamic library files (.so)
LIB_DIR := lib

CC := /usr/bin/gcc-12
# Common compiler warning flags
CFLAGS_WARN := -Wall -Wextra -Werror -Wdouble-promotion -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wwrite-strings -Wformat=2 -Wformat-overflow
# Compiler flags related to linking (i.e. extra libraries)
CFLAGS_LINK := 
# Compiler flags related to outside (i.e. system) headers
CFLAGS_INC := -I$(INCLUDE_DIR)
# Miscellaneous compiler flags
CFLAGS_MISC := -std=gnu2x -O3

# ==== VARIABLES ====
SHELL := bash
.ONESHELL:
.SHELLFLAGS := -eu -o pipefail -c
.DELETE_ON_ERROR:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

# Compiler flags for all build steps
CFLAGS := $(CFLAGS_INC) $(CFLAGS_MISC) $(CFLAGS_WARN)

# All source files
SRCS := $(shell find $(SRC_DIR)/ -type f -name "*.c")
# Respective object files
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
# Source file dependencies (.d)
DEPS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.d,$(SRCS))

# Include extern header files
ifneq ($(wildcard $(EXTERN_DIR)),)
	EXTERNS := $(shell find $(EXTERN_DIR) -maxdepth 1 -mindepth 1 -type d)
	EXTERN_INCLUDE := $(shell find $(EXTERNS) -maxdepth 1 -mindepth 1 -name include)
	CFLAGS += $(addprefix -I,$(EXTERN_INCLUDE))
endif

ifeq ($(BUILD_MODE),debug)
	CFLAGS += -ggdb
endif

# Link libraries in lib/
CFLAGS_LIBS = $(shell find $(LIB_DIR) -name "lib*" -type f -printf "-l:%f\n" 2>/dev/null)

# Target binary path
TARGET := $(BIN_DIR)/$(TARGET_EXEC)

# Compiler flags for objects
CFLAGS_OBJ :=

# Additional flags required for shared objects
ifeq ($(TARGET_LIBRARY),on)
	CFLAGS_OBJ += -fPIC
	CFLAGS_LINK += -shared
endif

# ==== RECIPES ====

.PHONY: all run debug time clean extern clean-all confirm-dialog

all: $(TARGET)

# Final build step (linking)
$(TARGET): extern $(DEPS) $(OBJS)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(OBJS) -o $@ -L$(LIB_DIR) $(CFLAGS_LIBS) $(CFLAGS_LINK)
# ^ Needed to link library dir

run: all
	export LD_LIBRARY_PATH=$(LIB_DIR)
	$(TARGET)

debug: all
	export LD_LIBRARY_PATH=$(LIB_DIR)
	gdb -q $(TARGET)

time: all
	export LD_LIBRARY_PATH=$(LIB_DIR)
	time $(TARGET)

# Build external libraries (may be hardcoded)
extern:

# Build objects
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(CFLAGS_OBJ) -c $< -o $@

# Build dependency files
$(OBJ_DIR)/%.d: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) -MT $(@D)/$(*F).o -MM $(SRC_DIR)/$*.c -MF $@

# Clean files related to intermediate build steps
clean:
	$(RM) -r $(OBJ_DIR)/

# Clean files related to
#  * Intermediate build steps (.o) (.s)
#  * Target binaries (main) (main.exe) (libmain.so)
#  * Libraries (liba.so)
# (prompts for confirmation)
clean-all: confirm-dialog
	$(RM) -r $(OBJ_DIR)/
	$(RM) -r $(BIN_DIR)/
	$(RM) -r $(LIB_DIR)/

confirm-dialog:
	@echo "This will remove the following:"
	@echo " * Intermediate build steps ($(OBJ_DIR))"
	@echo " * Target binaries ($(BIN_DIR))"
	@echo " * Libraries ($(LIB_DIR))"
	@echo -n "Are you sure? [y/N] " && read ans && [ $${ans:-N} = y ]

# Do not require source dependencies (.d) if only cleaning
ifeq (,$(filter clean%,$(MAKECMDGOALS)))
include $(DEPS)
endif