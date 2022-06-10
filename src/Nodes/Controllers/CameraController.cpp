#include "CameraController.hpp"

namespace mge {
    MGE_NODE(CameraController)
    MGE_ADD_PROPERTY(CameraController, Vector3, center, PUBLIC)
    MGE_ADD_PROPERTY(CameraController, float32_t, sensitivity, PUBLIC)
    MGE_END_NODE(CameraController)
}