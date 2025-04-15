#pragma once
#include <vulkan/vulkan.h>
#include "camera/OceanCamera.h"

//std
#include <vector>
#include <memory>

namespace ocean
{
    struct OceanFrameInfo
    {
        /* data */
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        PerspectiveCamera camera;
    };
    
} // namespace ocean

