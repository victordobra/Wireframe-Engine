#include "Renderer.hpp"

namespace mge {
    void Renderer::Start() {
        body = dynamic_cast<Body*>(GetParent());
        
        assert(body && "A renderer's parent must be a body!");
    }

    MGE_NODE(Renderer)
    MGE_ADD_NODE_PROPERTY(material, PUBLIC)
    MGE_END_NODE(Renderer)
}