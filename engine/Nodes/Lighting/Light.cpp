#include "Light.hpp"

namespace wfe {
    vector<Light*> Light::lights = {};
    Vector4 Light::ambientLightColor = {1.f, 1.f, 1.f, .01f};

    WFE_NODE(Light)
    WFE_ADD_NODE_PROPERTY(type, PUBLIC)
    WFE_ADD_NODE_PROPERTY(position, PUBLIC)
    WFE_ADD_NODE_PROPERTY(direction, PUBLIC)
    WFE_ADD_NODE_PROPERTY(color, PUBLIC)
    WFE_END_NODE(Light)
}