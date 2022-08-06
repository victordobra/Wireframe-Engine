#pragma once

#include "ECS/EngineECS.hpp"
#include "LightingUbo.hpp"

namespace wfe {
    class Light : public Component {
    public:
        Light() = default;
        Light(const Light&) = delete;
        Light(Light&&) noexcept = delete;

        Light& operator=(const Light&) = delete;
        Light& operator=(Light&&) noexcept = delete;

        /// @brief Called when a light's data needs to be written to the lighting ubo.
        virtual void WriteToLightingUbo(LightingUbo& ubo) { }

        virtual ~Light() = default;
    };
}