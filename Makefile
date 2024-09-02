# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Iincludes -std=c++20 -MMD -MP

# Linker flags
LDFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi

# Directories
INCLUDES = includes
SRCS_DIR = srcs
BUILD_DIR = build

# Create directories if they don't exist
$(shell mkdir -p $(BUILD_DIR))

# Source files
SRCS = \
    $(SRCS_DIR)/main.cpp 

# Object files
OBJS = $(patsubst $(SRCS_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Dependency files
DEPS = $(patsubst $(BUILD_DIR)/%.o,$(BUILD_DIR)/%.d,$(OBJS))

# Output executable
TARGET = scop

# Rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)
	@printf "\e[95m\e[0;94mCompiled \e[1m\e[95m\e[92m$(TARGET)\e[0m\n"

$(BUILD_DIR)/%.o: $(SRCS_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@mkdir -p $(dir $@)

# Include the generated dependency files
-include $(DEPS)

clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/*.d $(TARGET)

re: clean all

run: all
	./$(TARGET)

.PHONY: all clean re run
