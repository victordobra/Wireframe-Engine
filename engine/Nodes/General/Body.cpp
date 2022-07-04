#include "Body.hpp"

namespace wfe {
    WFE_NODE(Body)
    WFE_ADD_NODE_PROPERTY(position, PUBLIC)
    WFE_ADD_NODE_PROPERTY(rotation, PUBLIC)
    WFE_ADD_NODE_PROPERTY(scale, PUBLIC)
    WFE_END_NODE(Body)
}