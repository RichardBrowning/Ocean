#include "core/App.h"
#include <cstdlib>
#include <iostream>
#include <stdexcept>

// #ifdef _WIN32
// #define NOMINMAX
// #include <windows.h>

// // Windows entry point for GUI apps
// int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
// #else
// #endif
int main()
{
    try{
    //create app instance
    ocean::App app{};
    //run app
        app.run();
    // catch and throw error
    } catch(const std::exception& e) {
        std::wcerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}