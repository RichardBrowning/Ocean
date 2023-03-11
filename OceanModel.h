#pragma once
#include "OceanDevice.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <vector>

namespace ocean
{
    class OceanModel
    {
    public:
        struct Vertex
        {
            glm::vec2 position;

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };
        
        OceanModel(OceanDevice &device, const std::vector<Vertex> &vertices);
        ~OceanModel();
    
        OceanModel(const OceanModel&) = delete;
        OceanModel& operator=(const OceanModel&) = delete;

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);
    private:

        void createVertexBuffer(const std::vector<Vertex> &vertices);
        OceanDevice oceanDevice;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        uint32_t vertexCount;
    };
    
} // namespace ocean
