#pragma once
#include "OceanWindow.h"
#include "OceanDevice.h"
#include "OceanSwapChain.h"
#include "OceanPipeline.h"
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
            void loadGameObjects();
            void createPipelineLayout();
            void createPipeline();
            void createCommandBuffers();
            void freeCommandBuffers();
            void drawFrame();

            void recreateSwapChain();
            void recordCommandBuffer( int imageIndex);
            void renderGameObjects(VkCommandBuffer commandBuffer);

            OceanWindow oceanWindow{WIDTH, HEIGHT, NAME};
            OceanDevice oceanDevice{oceanWindow};
            // OceanSwapChain oceanSwapChain{oceanDevice, oceanWindow.getExtent()};
            std::unique_ptr<OceanSwapChain> oceanSwapChain;
            std::unique_ptr<OceanPipeline> oceanPipeline; //{device, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", OceanPipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
            VkPipelineLayout pipelineLayout;
            std::vector<VkCommandBuffer> commandBuffers;
            std::vector<OceanGameObject> gameObjects;
    };
}