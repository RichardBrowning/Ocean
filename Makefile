	
# C++ compiler flags
# -Wall: enable all warnings
# -g: enable debug symbols
# -std=c++17: use C++17 standard
# -I.: add current directory to include path
# -I$(VULKAN_SDK)/include: add Vulkan SDK include path to include path
# -I$(GLFW_PATH): add GLFW include path to include path

# LDFLAGS：gcc compiler optimization parameters, and also library files when necessary
# -L$(VULKAN_SDK)/lib: add Vulkan SDK lib path to library path
# -lvulkan: link to Vulkan library
# -mconsole: tells MingW to use main() as entry point instead of wWinMain() TODO: migrate to use cl.exe in vs project

ifeq ($(OS),Windows_NT)
	CXXFLAGS = -Wall -g -std=c++17 -Isrc -Iexternal -I"$(VULKAN_SDK)/include" -IC:\glfw-3.4.bin.WIN64\include
	LDFLAGS = -L"$(VULKAN_SDK)/lib" -LC:\glfw-3.4.bin.WIN64\lib-mingw-w64 -lglfw3 -lvulkan-1 -lgdi32 -static-libgcc -static-libstdc++
	GLSLC = $(VULKAN_SDK)/Bin/glslc.exe
	RM = del /Q
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		CXXFLAGS = -Wall -g -std=c++17 -Isrc -Iexternal -I/usr/include -I/usr/local/include
		LDFLAGS = -L/usr/lib -L/usr/local/lib `pkg-config --static glfw3` -lglfw -lvulkan -ldl -lpthread -lX11 -lXrandr -lXi
		GLSLC = glslc
		RM = rm -f
	endif
	ifeq ($(UNAME_S),Darwin)
		CXXFLAGS = -Wall -g -std=c++17 -Isrc -Iexternal -I$(VULKAN_SDK)/macOS/include -I$(CPATH)
		LDFLAGS = -L$(LIBRARY_PATH) -L$(VULKAN_SDK)/macOS/lib `pkg-config --static glfw3` -lglfw -lvulkan
		GLSLC = $(VULKAN_SDK)/macOS/bin/glslc
		RM = rm -f
	endif
endif

# compile the shaders
SHADER_DIR = shaders
ifeq ($(OS),Windows_NT)
	# Use wildcard for Windows (cmd doesn't support Unix-style find)
	SOURCES := $(wildcard src/*.cpp) \
			$(wildcard src/core/*.cpp) \
			$(wildcard src/render/*.cpp) \
			$(wildcard src/camera/*.cpp) \
			$(wildcard src/input/*.cpp) \
			$(wildcard src/particles/*.cpp)

	HEADERS := $(wildcard src/*.h) \
			$(wildcard src/core/*.h) \
			$(wildcard src/render/*.h) \
			$(wildcard src/camera/*.h) \
			$(wildcard src/input/*.h) \
			$(wildcard src/particles/*.h)

	VERT_SRC := $(wildcard shaders/*.vert)
	FRAG_SRC := $(wildcard shaders/*.frag)
else
	# Use find for Unix/macOS
	SOURCES := $(shell find src -name "*.cpp")
	HEADERS := $(shell find src -name "*.h")
	VERT_SRC := $(shell find shaders -name "*.vert")
	FRAG_SRC := $(shell find shaders -name "*.frag")
endif

SPV_VERT := $(patsubst %.vert, %.vert.spv, $(VERT_SRC))
SPV_FRAG := $(patsubst %.frag, %.frag.spv, $(FRAG_SRC))


# $@ macro @ evaluates to the name of the current target: all
# $< macro < evaluates to the name of the first prerequisite
# $^ macro ^ evaluates to the names of all the prerequisites


# Target
TARGET = build/main

# Build rule
$(TARGET): $(SOURCES) $(HEADERS) $(SPV_VERT) $(SPV_FRAG)
	g++ $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)

# Shader rule
%.spv: %
	$(GLSLC) $< -o $@

clean:
	$(RM) build/main
	$(RM) $(SPV_VERT) $(SPV_FRAG)

.PHONY: run clean

run: $(TARGET)
	./$(TARGET)