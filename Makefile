# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Iincludes -std=c++20 -MMD -MP

# Linker flags
LDFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi

# Directories
SRCS_DIR = src
BUILD_DIR = build
OTHERS_DIR = others
GLAD_DIR = $(OTHERS_DIR)/GLAD

INCLUDE_DIRS = -I./include -I./$(GLAD_DIR)/include -I./$(OTHERS_DIR)/stb

# Create directories if they don't exist
$(shell mkdir -p $(BUILD_DIR))

# Source files
SRCS = \
	$(SRCS_DIR)/main.cpp \
	$(SRCS_DIR)/Shader.cpp \
	$(SRCS_DIR)/Matrix4x4.cpp \
	$(SRCS_DIR)/Vector3.cpp \
	$(SRCS_DIR)/Vector4.cpp \
	others/GLAD/src/glad.c 


# Object files
OBJS = $(patsubst $(SRCS_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(patsubst $(GLAD_DIR)/src/%.c,$(BUILD_DIR)/%.o,$(SRCS)))


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
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@
	@mkdir -p $(dir $@)

$(BUILD_DIR)/%.o: $(GLAD_DIR)/src/%.c
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@
	@mkdir -p $(dir $@)


# Include the generated dependency files
-include $(DEPS)

clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/*.d $(TARGET)

re: clean all

run: all
	./$(TARGET)

.PHONY: all clean re run
