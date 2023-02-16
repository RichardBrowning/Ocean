#include "OceanWindow.h"

namespace ocean{
    //LESSON: std::initializer_list for const variables initialization
    OceanWindow::OceanWindow(int width, int height, std::string name) : width{width}, height{height}, name{name}
    {
        initWindow();
    }

    void OceanWindow::initWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        //LESSON: glfwCreateWindow() returns a pointer to a GLFWwindow object
        window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    }

    OceanWindow::~OceanWindow()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    void OceanWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
    {
        if(glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS){
            throw std::runtime_error("failed to create window surface!");
        }
    }
}