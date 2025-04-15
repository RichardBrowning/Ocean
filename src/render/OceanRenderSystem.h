#pragma once
#include "core/OceanDevice.h"
#include "core/OceanPipeline.h"
#include "OceanGameObject.h"
#include "camera/PerspectiveCamera.h"
#include "core/OceanFrameInfo.h"
#include <memory>
#include <vector>

namespace ocean {
    class OceanRenderSystem {
        public:
            OceanRenderSystem(OceanDevice &device, VkRenderPass renderPass);
            ~OceanRenderSystem();
            //delete copy constructor and copy assignment operator
            OceanRenderSystem(const OceanRenderSystem&) = delete;
            OceanRenderSystem& operator = (const OceanRenderSystem&) = delete;
            void renderGameObjects(
                OceanFrameInfo &frameInfo, 
                std::vector<OceanGameObject> &gameObjects
                // const PerspectiveCamera &camera
            );
        private:
            OceanDevice &oceanDevice;
            VkPipelineLayout pipelineLayout;
            std::unique_ptr<OceanPipeline> oceanPipeline; //{device, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", OceanPipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};

            void createPipelineLayout();
            void createPipeline(VkRenderPass renderPass);
    };
}