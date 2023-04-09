#pragma once
#include "OceanWindow.h"
#include "OceanDevice.h"
#include "OceanRenderer.h"
#include "OceanGameObject.h"
#include <memory>
#include <vector>

namespace ocean {
    class FirstApp {
        public:
            static constexpr int WIDTH = 800;
            static constexpr int HEIGHT = 600;
            const std::string NAME = "First App";
            FirstApp();
            ~FirstApp();

            //delete copy constructor and copy assignment operator
            FirstApp(const FirstApp&) = delete;
            FirstApp& operator = (const FirstApp&) = delete;

            void run();
        private:
            OceanWindow oceanWindow{WIDTH, HEIGHT, NAME};
            OceanDevice oceanDevice{oceanWindow};
            OceanRenderer oceanRenderer{oceanWindow, oceanDevice};
            //oceanSwapchain
            //oceanPipeline
            //pipelineLayout
            //commandBuffers
            std::vector<OceanGameObject> gameObjects;
            
            void loadGameObjects();
    };
}