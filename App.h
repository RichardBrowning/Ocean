#pragma once
#include "OceanWindow.h"
#include "OceanDevice.h"
#include "OceanRenderer.h"
#include "OceanGameObject.h"
#include <memory>
#include <vector>

namespace ocean {
    class App {
        public:
            static constexpr int WIDTH = 800;
            static constexpr int HEIGHT = 600;
            const std::string NAME = "First App";
            App();
            ~App();

            //delete copy constructor and copy assignment operator
            App(const App&) = delete;
            App& operator = (const App&) = delete;

            void run();
        private:
            OceanWindow oceanWindow = OceanWindow(WIDTH, HEIGHT, NAME);
            OceanDevice oceanDevice = OceanDevice(oceanWindow);
            OceanRenderer oceanRenderer = OceanRenderer(oceanWindow, oceanDevice);
            //oceanSwapchain
            //oceanPipeline
            //pipelineLayout
            //commandBuffers
            std::vector<OceanGameObject> gameObjects;
            
            void loadGameObjects();
    };
}