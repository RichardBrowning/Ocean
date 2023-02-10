#include "OceanWindow.hpp"

namespace ocean{
    //LESSON: std::initializer_list for const variables initialization
    OceanWindow::OceanWindow(int width, int height, std::string name) : width{width}, height{height}, name{name}
    {
        initWindow();
    }
    OceanWindow::~OceanWindow()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}