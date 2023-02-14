CFLAGS = -Wall -g -std=c++17 -I. -I$(VULKAN_SDK)/include

LDFLAGS = -L$(VULKAN_SDK)/lib `pkg-config --static --libs glfw3` -lvulkan

all: main

main: main.cpp
# $@ macro @ evaluates to the name of the current target: all
# $< macro < evaluates to the name of the first prerequisite
# $^ macro ^ evaluates to the names of all the prerequisites
	g++ $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f main

.PHONY: test clean

test: main
	./main