#include "Light.hpp"

namespace mge {
    vector<Light*> Light::lights = {};
    Vector4 Light::ambientLightColor = {1.f, 1.f, 1.f, .01f};

    MGE_NODE(Light)
    MGE_ADD_NODE_PROPERTY(type, PUBLIC)
    MGE_ADD_NODE_PROPERTY(position, PUBLIC)
    MGE_ADD_NODE_PROPERTY(direction, PUBLIC)
    MGE_ADD_NODE_PROPERTY(color, PUBLIC)
    MGE_END_NODE(Light)
}