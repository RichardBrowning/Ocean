#pragma once
#include "OceanWindow.h"
#include "OceanDevice.h"
#include "OceanRenderer.h"
#include "OceanGameObject.h"
#include <memory>
#include <vector>
#include <glm/glm.hpp>

namespace ocean {
    class App {
        public:
            App();
            ~App();
            glm::vec2 getSize();
            std::string getName();
            void run();
            struct GameObjectArray {
                std::string filePath;
                glm::vec3 translation;
                glm::vec3 scale;
            };
        private:
            const uint32_t WIDTH = 800;
            const uint32_t HEIGHT = 600;
            const std::string NAME = "Engine Demo";
            
            OceanWindow oceanWindow = OceanWindow(WIDTH, HEIGHT, NAME);
            OceanDevice oceanDevice = OceanDevice(oceanWindow);
            OceanRenderer oceanRenderer = OceanRenderer(oceanWindow, oceanDevice);
            //oceanSwapchain
            //oceanPipeline
            //pipelineLayout
            //commandBuffers
            std::vector<OceanGameObject> gameObjects;
            OceanGameObject singleGameObject(std::string filePath, glm::vec3 trainslation, glm::vec3 scale);
            void loadGameObjects();
    };
}