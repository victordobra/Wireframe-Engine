#include "CameraController.hpp"

namespace wfe {
    WFE_NODE(CameraController)
    WFE_ADD_NODE_PROPERTY(center, PUBLIC)
    WFE_ADD_NODE_PROPERTY(sensitivity, PUBLIC)
    WFE_END_NODE(CameraController)
}