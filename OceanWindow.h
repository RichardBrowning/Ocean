#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <stdexcept>

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

            //LESSON: delete copy constructor and copy assignment operator
            //dont want to have a dangling pointer when the copy is destructed
            OceanWindow(const OceanWindow&) = delete;
            OceanWindow& operator=(const OceanWindow&) = delete;

            bool shouldClose() { return glfwWindowShouldClose(window); };
            VkExtent2D getExtent() {return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};}
            
            //VkInstance is , VkSurfaceKHR* points to a vulkan surface 
            void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
    };
}