# C++ compiler flags
# -Wall: enable all warnings
# -g: enable debug symbols
# -std=c++17: use C++17 standard
# -I.: add current directory to include path
# -I$(VULKAN_SDK)/include: add Vulkan SDK include path to include path
# -I$(GLFW_PATH): add GLFW include path to include path
CXXFLAGS = -Wall -g -std=c++17 -I. -I$(VULKAN_SDK)/include -IC:\glfw-3.3.8.bin.WIN64/include

# LDFLAGS：gcc 等编译器会用到的一些优化参数，也可以在里面指定库文件的位置
# -L$(VULKAN_SDK)/lib: add Vulkan SDK lib path to library path
# -lvulkan: link to Vulkan library

LDFLAGS = -L$(VULKAN_SDK)/lib
# `pkg-config --static glfw3` -lvulkan 

all: main

main: *.cpp *.h
# $@ macro @ evaluates to the name of the current target: all
# $< macro < evaluates to the name of the first prerequisite
# $^ macro ^ evaluates to the names of all the prerequisites
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f main

.PHONY: test clean

test: main
	./main