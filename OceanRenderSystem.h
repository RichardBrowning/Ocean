#pragma once
#include "OceanDevice.h"
#include "OceanPipeline.h"
#include "OceanGameObject.h"
#include "OceanPerspectiveCamera.h"
#include <memory>
#include <vector>

namespace ocean {
    class OceanRenderSystem {
        public:
        OceanRenderSystem(OceanDevice &device, VkRenderPass renderPass);
        ~OceanRenderSystem();

        OceanRenderSystem(const OceanRenderSystem &) = delete;
        OceanRenderSystem &operator=(const OceanRenderSystem &) = delete;

        void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<OceanGameObject> &gameObjects, const OceanPerspectiveCamera &camera);

        private:
        void createPipelineLayout();
        void createPipeline(VkRenderPass renderPass);

        OceanDevice &oceanDevice;

        std::unique_ptr<OceanPipeline> oceanPipeline;
        VkPipelineLayout pipelineLayout;
    };
}