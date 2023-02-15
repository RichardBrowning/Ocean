#pragma once

#include "OceanWindow.h"
#include "OceanPipeline.h"

namespace ocean {
    class FirstApp {
        public:
            static constexpr int WIDTH = 800;
            static constexpr int HEIGHT = 600;
            const std::string NAME = "First App";

            void run();
        private:
           OceanWindow oceanWindow{WIDTH, HEIGHT, NAME};
           OceanPipeline oceanPipeline{"shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv"};
    };
}