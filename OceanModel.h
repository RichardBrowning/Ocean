#pragma once
#include "OceanDevice.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

namespace ocean
{
    class OceanModel
    {
    public:
        struct Vertex
        {
            glm::vec3 position{};
            glm::vec3 color{};

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };

        struct Builder{
            std::vector<Vertex> vertices{};
            std::vector<uint32_t> indices{};
        };
        
        OceanModel(OceanDevice &device, const Builder &builder);
        ~OceanModel();
    
        OceanModel(const OceanModel&) = delete;
        OceanModel& operator=(const OceanModel&) = delete;

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);
    private:
        void createVertexBuffer(const std::vector<Vertex> &vertices);
        void createIndexBuffer(const std::vector<uint32_t> &indices);
        OceanDevice &oceanDevice; //reference to the device
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        uint32_t vertexCount;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;
        uint32_t indexCount;
        bool hasIndexBuffer{false};
    };
    
} // namespace ocean
