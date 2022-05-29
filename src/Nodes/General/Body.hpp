#pragma once
#include "Base/Node.hpp"
#include "Math/EngineMath.hpp"

namespace mge {
    class Body : public Node {
    public:
        Vector3 position;
        Quaternion rotation;
        Vector3 scale;

        virtual void UpdateBody() { }
        
        void Update() override { UpdateBody(); }
    };
}