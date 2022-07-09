#include "Camera.hpp"
#include "RenderingSystem.hpp"

namespace wfe {
    WFE_COMPONENT(Camera)
    WFE_COMPONENT_DEFAULT_SYSTEM(GetRenderingSystem)
    WFE_COMPONENT_PROPERTY(fov, PUBLIC)
    WFE_COMPONENT_PROPERTY(nearPlane, PUBLIC)
    WFE_COMPONENT_PROPERTY(farPlane, PUBLIC)
    WFE_COMPONENT_PROPERTY(clearColor, PUBLIC)
    WFE_END_COMPONENT(Camera)
}