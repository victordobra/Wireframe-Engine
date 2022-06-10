#include "Light.hpp"

namespace mge {
    vector<Light*> Light::lights = {};
    Vector4 Light::ambientLightColor = {1.f, 1.f, 1.f, .05f};

    MGE_NODE(Light)
    MGE_ADD_PROPERTY(Light, Light::LightType, type, PUBLIC)
    MGE_ADD_PROPERTY(Light, Vector3, position, PUBLIC)
    MGE_ADD_PROPERTY(Light, Vector3, direction, PUBLIC)
    MGE_ADD_PROPERTY(Light, Vector4, color, PUBLIC)
    MGE_END_NODE(Light)
}