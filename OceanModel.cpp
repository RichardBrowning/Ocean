#include "OceanModel.h"
#include "OceanHashCombine.h"
#include <cassert>
#include <cstring>
#include <iostream>
#include <unordered_map>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

namespace std{
    template <>
    struct hash<ocean::OceanModel::Vertex>{
        size_t operator()(ocean::OceanModel::Vertex const& vertex) const{
            size_t seed = 0;
            //hash function to hash the vertex data
            ocean::hashCombine(seed, vertex.position, vertex.normal, vertex.color, vertex.uv);
            return seed;
        }
    };
}

namespace ocean {
    OceanModel::OceanModel(OceanDevice &device, const OceanModel::Builder &builder) : oceanDevice{device} //initialize "device" member variable
    {
        createVertexBuffer(builder.vertices);
        createIndexBuffer(builder.indices);
    }

    OceanModel::~OceanModel()
    {
        vkDestroyBuffer(oceanDevice.device(), vertexBuffer, nullptr);
        vkFreeMemory(oceanDevice.device(), vertexBufferMemory, nullptr);
        if (hasIndexBuffer){
            vkDestroyBuffer(oceanDevice.device(), indexBuffer, nullptr);
            vkFreeMemory(oceanDevice.device(), indexBufferMemory, nullptr);
        }
    }
    void OceanModel::createIndexBuffer(const std::vector<uint32_t> &indices){
        indexCount = static_cast<uint32_t>(indices.size());
        if (indexCount != 0)
            hasIndexBuffer = true;
        if (!hasIndexBuffer)
            return;
        VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        oceanDevice.createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingBufferMemory);

        void *data;
        vkMapMemory(oceanDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(oceanDevice.device(), stagingBufferMemory);

        oceanDevice.createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            indexBuffer,
            indexBufferMemory);

        //copy staging buffer to index buffer
        oceanDevice.copyBuffer(stagingBuffer, indexBuffer, bufferSize);

        //cleanup staging buffer
        vkDestroyBuffer(oceanDevice.device(), stagingBuffer, nullptr);
        vkFreeMemory(oceanDevice.device(), stagingBufferMemory, nullptr);
    }

    void OceanModel::createVertexBuffer(const std::vector<Vertex> &vertices){
        vertexCount = static_cast<uint32_t>(vertices.size());
        assert(vertexCount >= 3 && "Vertex count must be at least 3");
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        oceanDevice.createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingBufferMemory);

        void *data;
        vkMapMemory(oceanDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(oceanDevice.device(), stagingBufferMemory);

        oceanDevice.createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            vertexBuffer,
            vertexBufferMemory);

        //copy staging buffer to vertex buffer
        oceanDevice.copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

        //cleanup staging buffer
        vkDestroyBuffer(oceanDevice.device(), stagingBuffer, nullptr);
        vkFreeMemory(oceanDevice.device(), stagingBufferMemory, nullptr);
    }

    void OceanModel::bind(VkCommandBuffer commandBuffer){
        VkBuffer vertexBuffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        if (hasIndexBuffer)
            vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    }

    void OceanModel::draw(VkCommandBuffer commandBuffer){
        if (hasIndexBuffer)
            vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
        else
            vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
    }


    std::vector<VkVertexInputBindingDescription> OceanModel::Vertex::getBindingDescriptions()
    {
        std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescriptions;
    }

    std::vector<VkVertexInputAttributeDescription> OceanModel::Vertex::getAttributeDescriptions()
    {
        std::vector<VkVertexInputAttributeDescription> inputAttributeDescription(2);
        inputAttributeDescription[0].binding = 0;
        inputAttributeDescription[0].location = 0;
        inputAttributeDescription[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        inputAttributeDescription[0].offset = offsetof(Vertex, position);

        inputAttributeDescription[1].binding = 0;
        inputAttributeDescription[1].location = 1;
        inputAttributeDescription[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        inputAttributeDescription[1].offset = offsetof(Vertex, color);//calculates the offset from the first element of the structure to the color element
        return inputAttributeDescription;
    }

    std::unique_ptr<OceanModel> OceanModel::createModel(OceanDevice &device, const std::string &filePath){
        Builder builder{};
        builder.loadModel(filePath);
        //std::cout << "Vertex count: " << builder.vertices.size() << std::endl;
        //std::cout << "Index count: " << builder.indices.size() << std::endl;
        return std::make_unique<OceanModel>(device, builder);
    }
    void OceanModel::Builder::loadModel(const std::string &filePath)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filePath.c_str()))
            throw std::runtime_error(warn + err);

        vertices.clear();
        indices.clear();

        std::unordered_map<Vertex, uint32_t> uniqueVertices = {};
        for (const auto &shape : shapes)
        {
            for (const auto &index : shape.mesh.indices)//loop through all elements of the mesh, return the value of the index
            {
                //init a vertex
                Vertex vertex{};
                if (index.vertex_index >= 0){//vertex_index is the first value of the face element: what position value to use, if -1, not provided
                    vertex.position = {
                        attrib.vertices[3 * index.vertex_index], 
                        attrib.vertices[3 * index.vertex_index + 1], 
                        attrib.vertices[3 * index.vertex_index + 2]
                    };

                    auto colorIndex = 3 * index.vertex_index + 2;
                    if (colorIndex < attrib.colors.size()){
                        vertex.color = {
                            attrib.colors[colorIndex - 2],
                            attrib.colors[colorIndex - 1],
                            attrib.colors[colorIndex]};
                    }else
                        vertex.color = {1.0f, 1.0f, 1.0f};
                }

                if (index.normal_index >= 0)
                {
                    vertex.normal = {
                        attrib.normals[3 * index.normal_index],
                        attrib.normals[3 * index.normal_index + 1],
                        attrib.normals[3 * index.normal_index + 2]};
                }
                if (index.texcoord_index >= 0)
                {
                    vertex.uv = {
                        attrib.texcoords[2 * index.texcoord_index],
                        attrib.texcoords[2 * index.texcoord_index + 1]};
                }

                if (uniqueVertices.count(vertex) == 0)//if vertex is new, add to the map
                {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }

                indices.push_back(uniqueVertices[vertex]);
            }
        }
    }
}