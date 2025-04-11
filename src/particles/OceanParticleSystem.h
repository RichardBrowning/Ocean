#pragma once
#include "OceanDevice.h"
#include "OceanSwapChain.h"
#include <glm/glm.hpp>

class OceanParticleSystem{
    struct Particle {
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec4 color;
        float lifetime;
    };
    public:
        OceanParticleSystem();
        ~OceanParticleSystem();
        // void createParticleBuffer(OceanDevice& device, uint32_t maxParticles);
        // void constructComputePipeline(OceanDevice& device, OceanSwapChain& swapChain);
        // void constructRenderPipeline(OceanDevice& device, OceanSwapChain& swapChain);
};