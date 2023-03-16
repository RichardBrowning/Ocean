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

            VkRenderPass getSwapChainRenderPass() const { return oceanSwapChain->getRenderPass(); }

            bool isFrameInProgress() const { return isFrameStarted; }
            
            VkCommandBuffer getCurrentCommandBuffer() const { 
                assert(isFrameStarted && "Frame is not started, cannot get command buffer!");
                return commandBuffers[currentFrameIndex]; //this typo wasted 6 hours of my life
            }
            int getFrameIndex() const {
                assert(isFrameStarted && "Cannot get frame index when frame not in progress!");
                return currentFrameIndex;
            }
            //start frame
            VkCommandBuffer beginFrame();
            //end frame
            void endFrame();
            void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
            void endSwapChainRenderPass(VkCommandBuffer commandBuffer);
        private:
            uint32_t currentImageIndex;
            int currentFrameIndex;
            bool isFrameStarted{false};

            void createCommandBuffers();
            void freeCommandBuffers();
            void recreateSwapChain();

            OceanWindow &oceanWindow;
            OceanDevice &oceanDevice;
            std::unique_ptr<OceanSwapChain> oceanSwapChain;
            std::vector<VkCommandBuffer> commandBuffers;
    };
}