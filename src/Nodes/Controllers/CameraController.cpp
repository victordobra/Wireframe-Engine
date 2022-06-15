#include "CameraController.hpp"

namespace mge {
    MGE_NODE(CameraController)
    MGE_ADD_NODE_PROPERTY(center, PUBLIC)
    MGE_ADD_NODE_PROPERTY(sensitivity, PUBLIC)
    MGE_END_NODE(CameraController)
}