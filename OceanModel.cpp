#include "OceanModel.h"
#include <cassert>

namespace ocean {
    OceanModel::OceanModel(OceanDevice &device, const std::vector<Vertex> &vertices) : oceanDevice{device}
    {
        createVertexBuffer(vertices);
    }

    OceanModel::~OceanModel()
    {
        vkDestroyBuffer(oceanDevice.device(), vertexBuffer, nullptr);
        vkFreeMemory(oceanDevice.device(), vertexBufferMemory, nullptr);
    }

    void OceanModel::createVertexBuffer(const std::vector<Vertex> &vertices){
        vertexCount = static_cast<uint32_t>(vertices.size());
    }

    void OceanModel::bind(VkCommandBuffer commandBuffer)
    {
        ;
    }

    void OceanModel::draw(VkCommandBuffer commandBuffer)
    {
        ;
    }

}