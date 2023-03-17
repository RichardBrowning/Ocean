#pragma once
#include "OceanWindow.h"
#include "OceanDevice.h"
#include "OceanSwapChain.h"
#include <memory>
#include <vector>
#include <cassert>

namespace ocean {
    class OceanRenderer {
        public:
        OceanRenderer(OceanWindow &window, OceanDevice &device);
        ~OceanRenderer();

        OceanRenderer(const OceanRenderer &) = delete;
        OceanRenderer &operator=(const OceanRenderer &) = delete;

        VkRenderPass getSwapChainRenderPass() const { return oceanSwapChain->getRenderPass(); }
        bool isFrameInProgress() const { return isFrameStarted; }

        VkCommandBuffer getCurrentCommandBuffer() const {
            assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
            return commandBuffers[currentFrameIndex];
        }

        int getFrameIndex() const {
            assert(isFrameStarted && "Cannot get frame index when frame not in progress");
            return currentFrameIndex;
        }

        VkCommandBuffer beginFrame();
        void endFrame();
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

        private:
        void createCommandBuffers();
        void freeCommandBuffers();
        void recreateSwapChain();

        OceanWindow &oceanWindow;
        OceanDevice &oceanDevice;
        std::unique_ptr<OceanSwapChain> oceanSwapChain;
        std::vector<VkCommandBuffer> commandBuffers;

        uint32_t currentImageIndex;
        int currentFrameIndex;
        bool isFrameStarted;
    };
}