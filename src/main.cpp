#include "App.h"
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {
    //create app instance
    ocean::App app{};
    //run app
    try{
        app.run();
    // catch and throw error
    } catch(const std::exception& e) {
        std::wcerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}