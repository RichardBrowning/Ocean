	
# C++ compiler flags
# -Wall: enable all warnings
# -g: enable debug symbols
# -std=c++17: use C++17 standard
# -I.: add current directory to include path
# -I$(VULKAN_SDK)/include: add Vulkan SDK include path to include path
# -I$(GLFW_PATH): add GLFW include path to include path

# LDFLAGS：gcc 等编译器会用到的一些优化参数，也可以在里面指定库文件的位置
# -L$(VULKAN_SDK)/lib: add Vulkan SDK lib path to library path
# -lvulkan: link to Vulkan library

ifeq ($(OS),Windows_NT)
	CXXFLAGS = -Wall -g -std=c++17 -I. -I$(VULKAN_SDK)/include -IC:\glfw-3.3.8.bin.WIN64/include
	LDFLAGS = -L$(VULKAN_SDK)/lib -LC:\glfw-3.3.8.bin.WIN64\lib-mingw-w64 -lglfw3 -lvulkan-1 -lgdi32 -static-libgcc -static-libstdc++
	GLSLC = C:\VulkanSDK\Bin\glslc.exe
else
	UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CXXFLAGS = -Wall -g -std=c++17 -I. -I/usr/include -I/usr/local/include
		LDFLAGS = -L/usr/lib -L/usr/local/lib `pkg-config --static glfw3` -lglfw -lvulkan -ldl -lpthread -lX11 -lXrandr -lXi
		GLSLC = glslc
    endif
    ifeq ($(UNAME_S),Darwin)
        CXXFLAGS = -Wall -g -std=c++17 -I. -I$(VULKAN_SDK)/macOS/include -I$(CPATH)
		LDFLAGS = -L$(LIBRARY_PATH) -L$(VULKAN_SDK)/macOS/lib `pkg-config --static glfw3` -lglfw -lvulkan
		GLSLC = $(VULKAN_SDK)/macOS/bin/glslc
    endif
	
endif
# Windows: manually compile the shader
vertSource = $(shell find ./shaders -type f -name "*.vert")
fragSource = $(shell find ./shaders -type f -name "*.frag")
vertObjFiles = $(patsubst %.vert, %.vert.spv, $(vertSource))
fragObjFiles = $(patsubst %.frag, %.frag.spv, $(fragSource))

TARGET = main

$(TARGET): $(vertObjFiles) $(fragObjFiles)
$(TARGET): *.cpp *.h
# $@ macro @ evaluates to the name of the current target: all
# $< macro < evaluates to the name of the first prerequisite
# $^ macro ^ evaluates to the names of all the prerequisites
	g++ $(CXXFLAGS) -o $@ *.cpp $(LDFLAGS) 

#make shader target
%.spv: %
	$(GLSLC) $< -o $@

clean:
	rm -f main

.PHONY: test clean

run: main
	./main