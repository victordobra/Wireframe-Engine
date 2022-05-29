#include "Renderer.hpp"

namespace mge {
    Renderer::Renderer() {
        body = dynamic_cast<Body*>(GetParent());
        
        assert(body && "A renderer's parent must be a body!");
    }

    MGE_NODE(Renderer)
    MGE_END_NODE(Renderer)
}