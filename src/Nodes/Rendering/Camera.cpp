#include "Camera.hpp"

namespace mge {
    void Camera::Render() {

    }

    MGE_NODE(Camera)
    MGE_ADD_PROPERTY(Camera, Vector3, position, PUBLIC)
    MGE_ADD_PROPERTY(Camera, Quaternion, rotation, PUBLIC)
    MGE_ADD_PROPERTY(Camera, float32_t, fov, PUBLIC)
    MGE_ADD_PROPERTY(Camera, Color8, clearColor, PUBLIC)
    MGE_END_NODE(Camera)
}