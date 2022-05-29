#include "Body.hpp"

namespace mge {
    MGE_NODE(Body)
    MGE_ADD_PROPERTY(Body, Vector3, position, PUBLIC)
    MGE_ADD_PROPERTY(Body, Quaternion, rotation, PUBLIC)
    MGE_ADD_PROPERTY(Body, Vector3, scale, PUBLIC)
    MGE_END_NODE(Body)
}