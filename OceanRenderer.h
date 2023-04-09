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

            //delete copy constructor and copy assignment operator
            OceanRenderer(const OceanRenderer&) = delete;
            OceanRenderer& operator = (const OceanRenderer&) = delete;
            
            //begin frame
            VkCommandBuffer beginFrame();
            //end frame
            void endFrame();

            VkRenderPass getSwapChainRenderPass() const { return oceanSwapChain->getRenderPass(); }
            bool isFrameInProgress() const { return isFrameStarted; }

            VkCommandBuffer getCurrentCommandBuffer() const { 
                assert(isFrameStarted && "Frame is not started, cannot get command buffer!");
                return commandBuffers[currentFrameIndex]; }

            void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
            void endSwapChainRenderPass(VkCommandBuffer commandBuffer);
            int getFrameIndex() const {
                assert(isFrameStarted && "Cannot get command buffer when frame not in progress.");
                return currentFrameIndex;
            }
        private:
            uint32_t currentImageIndex;
            int currentFrameIndex{0};
            bool isFrameStarted{false};
            OceanWindow &oceanWindow;
            OceanDevice &oceanDevice;
            std::unique_ptr<OceanSwapChain> oceanSwapChain;
            std::vector<VkCommandBuffer> commandBuffers;

            void createCommandBuffers();
            void freeCommandBuffers();
            void recreateSwapChain();

    };
}