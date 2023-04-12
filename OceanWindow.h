#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <stdexcept>

namespace ocean {
    class OceanWindow {
        private:
             int width;
             int height;
             bool frameBufferResized = false;
            std::string name;
            GLFWwindow* window;
            void initWindow();
            static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
        public:
            OceanWindow(int width, int height, std::string name);
            ~OceanWindow();
            //LESSON: delete copy constructor and copy assignment operator
            //dont want to have a dangling pointer when the copy is destructed
            OceanWindow(const OceanWindow&) = delete;
            OceanWindow& operator=(const OceanWindow&) = delete;
            GLFWwindow* getWindow() { return window; }
            bool shouldClose() { return glfwWindowShouldClose(window); };
            VkExtent2D getExtent() {return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};}
            
            void resetWindowResizedFlag() { frameBufferResized = false; }
            bool wasWindowResized() { return frameBufferResized; }
            
            //VkInstance is , VkSurfaceKHR* points to a vulkan surface 
            void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
    };
}