#include "Renderer.hpp"

namespace mge {
    void Renderer::Start() {
        body = dynamic_cast<Body*>(GetParent());
        
        assert(body && "A renderer's parent must be a body!");
    }

    MGE_NODE(Renderer)
    MGE_ADD_PROPERTY(Renderer, Material*, material, PUBLIC)
    MGE_END_NODE(Renderer)
}