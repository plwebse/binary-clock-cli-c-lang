# ==============================================================================
# Compiler & Flags
# ==============================================================================
CC       := gcc
CFLAGS   := -Wall -Wextra -Werror -std=c99 -pedantic -g
LDFLAGS  := 

# ==============================================================================
# Installation Paths (Standard POSIX layout)
# ==============================================================================
PREFIX   ?= /usr/local
BINDIR   := $(PREFIX)/bin

# ==============================================================================
# Directories & Files
# ==============================================================================
SRC_DIR  := src
INC_DIR  := include
BUILD_DIR:= build
TARGET   := binary-clock

# Find all .c files in SRC_DIR and map them to .o object files in BUILD_DIR
SRCS     := $(wildcard $(SRC_DIR)/*.c)
OBJS     := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS     := $(OBJS:.o=.d)

# Include header directory in CFLAGS
CFLAGS   += -I$(INC_DIR)

# ==============================================================================
# Rules
# ==============================================================================
.PHONY: all clean fclean re run install uninstall

# Default target
all: $(TARGET)

# Link object files into executable
$(TARGET): $(OBJS)
	@echo "Linking $@..."
	$(CC) $(OBJS) $(LDFLAGS) -o $@
	@echo "Build complete: $@"

# Compile C source files into object files (-MMD generates .d header dependencies)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Create build directory if it doesn't exist
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Include automatically generated dependency files (.d)
-include $(DEPS)

# Run the compiled executable
run: all
	./$(TARGET)

# Remove intermediate build objects
clean:
	@echo "Cleaning object files..."
	@rm -rf $(BUILD_DIR)

# Remove all generated files (objects + target executable)
fclean: clean
	@echo "Cleaning executable..."
	@rm -f $(TARGET)

# Rebuild everything from scratch
re: fclean all

# Copy the executable to the system binary directory
install: $(TARGET)
	@echo "Installing $(TARGET) to $(DESTDIR)$(BINDIR)..."
	@mkdir -p $(DESTDIR)$(BINDIR)
	@install -m 755 $(TARGET) $(DESTDIR)$(BINDIR)/$(TARGET)
	@echo "Installation complete."

# Remove the executable from the system binary directory
uninstall:
	@echo "Removing $(TARGET) from $(DESTDIR)$(BINDIR)..."
	@rm -f $(DESTDIR)$(BINDIR)/$(TARGET)
	@echo "Uninstall complete."