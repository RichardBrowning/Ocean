#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <memory>

#include "core/OceanDevice.h"
#include "core/OceanBuffer.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

namespace ocean
{
    class OceanModel
    {
    public:
        struct Vertex
        {
            glm::vec3 position{};
            glm::vec3 color{};
            glm::vec3 normal{};
            glm::vec2 uv{};

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

            bool operator==(const Vertex &vertex) const{
                return (position == vertex.position && color == vertex.color && normal == vertex.normal && uv == vertex.uv);
            }
        };

        struct Builder{
            std::vector<Vertex> vertices{};
            std::vector<uint32_t> indices{};
            void loadModel(const std::string &filePath);
        };
        
        OceanModel(OceanDevice &device, const Builder &builder);
        ~OceanModel();
    
        OceanModel(const OceanModel&) = delete;
        OceanModel& operator=(const OceanModel&) = delete;

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);
        static std::unique_ptr<OceanModel> createModel(OceanDevice& device, const std::string &filePath);
    private:
        void createVertexBuffer(const std::vector<Vertex> &vertices);
        void createIndexBuffer(const std::vector<uint32_t> &indices);
        OceanDevice &oceanDevice; //reference to the device
        // uniform buffer object
        std::unique_ptr<OceanBuffer> vertexBuffer;
        // VkDeviceMemory vertexBufferMemory;
        uint32_t vertexCount;
        
        std::unique_ptr<OceanBuffer> indexBuffer;
        //VkDeviceMemory indexBufferMemory;
        uint32_t indexCount;
        bool hasIndexBuffer{false};
    };
    
} // namespace ocean
