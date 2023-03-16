#pragma once
#include "OceanWindow.h"
#include "OceanDevice.h"
#include "OceanGameObject.h"
#include "OceanRenderer.h"
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
            void loadGameObjects();

            OceanWindow oceanWindow{WIDTH, HEIGHT, NAME};
            OceanDevice oceanDevice{oceanWindow};
            OceanRenderer oceanRenderer{oceanWindow, oceanDevice};
            //std::unique_ptr<OceanSwapChain> oceanSwapChain;
            //std::unique_ptr<OceanPipeline> oceanPipeline; //{device, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", OceanPipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
            //VkPipelineLayout pipelineLayout;
            //std::vector<VkCommandBuffer> commandBuffers;
            std::vector<OceanGameObject> gameObjects;
    };
}