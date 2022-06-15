#include "Body.hpp"

namespace mge {
    MGE_NODE(Body)
    MGE_ADD_NODE_PROPERTY(position, PUBLIC)
    MGE_ADD_NODE_PROPERTY(rotation, PUBLIC)
    MGE_ADD_NODE_PROPERTY(scale, PUBLIC)
    MGE_END_NODE(Body)
}