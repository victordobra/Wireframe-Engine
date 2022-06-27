#pragma once

#include "Base/Node.hpp"
#include "Math/EngineMath.hpp"

namespace mge {
    class Body : public Node {
    public:
        Vector3 position;
        Quaternion rotation;
        Vector3 scale{ 1.f, 1.f, 1.f };

        Body() = default;
        Body(const Body&) = delete;
        Body(Body&&) noexcept = delete;

        Body& operator=(const Body&) = delete;
        Body& operator=(Body&&) noexcept = delete;

        virtual void UpdateBody() { }
        
        void Update() override { UpdateBody(); }

        ~Body() = default;
    };
}