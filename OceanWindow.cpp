#include "OceanWindow.h"

namespace ocean{
    //LESSON: std::initializer_list for const variables initialization
    OceanWindow::OceanWindow(int width, int height, std::string name) : width{width}, height{height}, name{name}
    {
        initWindow();
    }

    void OceanWindow::initWindow()
    {
        //glfw proces init
        glfwInit();
        //hint set
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        //glfwCreateWindow() returns a pointer to a GLFWwindow object
        window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    }

    OceanWindow::~OceanWindow()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    //window suface, like WebGL context, take a vk instance and a vk surface 
    void OceanWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
    {
        if(glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS){
            throw std::runtime_error("failed to create window surface!");
        }
    }

    void OceanWindow::framebufferResizeCallback(GLFWwindow *window, int width, int height)
    {
        auto oceanWindow = reinterpret_cast<OceanWindow*>(glfwGetWindowUserPointer(window));
        oceanWindow -> frameBufferResized = true;
        oceanWindow -> width = width;
        oceanWindow -> height = height;
    }
}