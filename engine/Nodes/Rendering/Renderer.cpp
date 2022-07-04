#include "Renderer.hpp"

namespace wfe {
    void Renderer::Start() {
        body = dynamic_cast<Body*>(GetParent());
        
        assert(body && "A renderer's parent must be a body!");
    }

    WFE_NODE(Renderer)
    WFE_ADD_NODE_PROPERTY(material, PUBLIC)
    WFE_END_NODE(Renderer)
}