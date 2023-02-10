#pragma once

#include <string>
#define GLFW_INCLUDE_VULKAN

# include <GLFW/glfw3.h>

namespace ocean {
    class OceanWindow {
        private:
            void initWindow();
            const int width;
            const int height;
            std::string name;
            GLFWwindow* window;
        public:
            OceanWindow(int width, int height, std::string name);
            ~OceanWindow();
    };
}