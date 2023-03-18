#include "OceanPerspectiveCamera.h"

namespace ocean{
    void OceanPerspectiveCamera::setOrthographicProjection(float left, float right, float top, float bottom, float near, float far)
    {
        projectionMatrix = glm::mat4{1.0f};
        projectionMatrix[0][0] = 2.f / (right - left);
        projectionMatrix[1][1] = 2.f / (bottom - top);
        projectionMatrix[2][2] = 1.f / (far - near);
        projectionMatrix[3][0] = -(right + left) / (right - left);
        projectionMatrix[3][1] = -(bottom + top) / (bottom - top);
        projectionMatrix[3][2] = - near / (far - near);
    }

    void OceanPerspectiveCamera::setPerspectiveProjection(float fovY, float aspect, float near, float far)
    {
        //Returns the machine epsilon, that is, the difference between 1.0 and the next value representable by the floating-point type T. It is only meaningful if std::numeric_limits<T>::is_integer == false
        //Here as long as aspect is not lim(->0), the assert is true
        assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
        const float tanHalfFovY = tan(fovY / 2.f);

        projectionMatrix = glm::mat4{0.0f};
        projectionMatrix[0][0] = 1.f / (aspect * tanHalfFovY);
        projectionMatrix[1][1] = 1.f / (tanHalfFovY);
        projectionMatrix[2][2] = far / (far - near);
        projectionMatrix[2][3] = 1.f;
        projectionMatrix[3][2] = -(far*near) / (far-near);

    }
}