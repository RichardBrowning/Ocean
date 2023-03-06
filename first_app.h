#pragma once
#include "OceanWindow.h"
#include "OceanDevice.h"
#include "OceanSwapChain.h"
#include "OceanPipeline.h"
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
            void operator = (const FirstApp&) = delete;

            void run();
        private:
            void createPipelineLayout();
            void createPipeline();
            void createCommandBuffers();
            void drawFrame();

            OceanWindow oceanWindow{WIDTH, HEIGHT, NAME};
            OceanDevice device{oceanWindow};
            OceanSwapChain swapChain{device, oceanWindow.getExtent()};
            std::unique_ptr<OceanPipeline> oceanPipeline;//{device, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", OceanPipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
            VkPipelineLayout pipelineLayout;
            std::vector<VkCommandBuffer> commandBuffers;
    };
}