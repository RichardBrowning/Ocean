#pragma once
#include "OceanModel.h"
#include <memory>

namespace ocean{
    struct Transform2dComponent {
        glm::vec2 translation {};
        glm::vec2 scale {1.f, 1.f};
        float rotation;
        glm::mat2 mat2() {
            const float sin = std::sin(rotation);
            const float cos = std::cos(rotation);
            glm::mat2 rotMat {{cos, -sin}, {sin, cos}};
            glm::mat2 scaleMat {{scale.x, 0.f}, {0.f, scale.y}};
            return rotMat * scaleMat;
        }
    };

    class OceanGameObject{
    public:
        using id_t = unsigned int;

        static OceanGameObject createGameObject(){
            static id_t currentId = 0;
            return OceanGameObject{currentId++};
        }
        OceanGameObject(const OceanGameObject& other) = delete;
        OceanGameObject& operator = (const OceanGameObject& other) = delete;
        OceanGameObject(OceanGameObject && ) = default;
        OceanGameObject& operator = (OceanGameObject &&) = default;

        id_t getId() const { return id; }
        
        std::shared_ptr<OceanModel> model{};
        glm::vec3 color{};
        Transform2dComponent transform2d{};

    private:
        OceanGameObject(id_t id) : id{id} {}
        id_t id;

    };
}