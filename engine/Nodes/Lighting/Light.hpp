#pragma once

#include "LightingUbo.hpp"
#include "Base/Node.hpp"

namespace wfe {
    class Light : public Node {
    public:
        static Vector4 ambientLightColor;
    
        typedef enum : uint8_t {
            LIGHT_TYPE_DIRECTIONAL,
            LIGHT_TYPE_POINT
        } LightType;

        LightType type{};
        Vector3 position{};
        Vector3 direction{};
        Vector4 color{};

        Light() : index(lights.size()) {
            lights.push_back(this);
        }
        Light(const Light&) = delete;
        Light(Light&&) noexcept = delete;

        Light& operator=(const Light&) = delete;
        Light& operator=(Light&&) noexcept = delete;

        void AppendToLightingUbo(LightingUbo& lightingUbo) const {
            switch(type) {
            case LIGHT_TYPE_DIRECTIONAL:
            {
                // Increment the directional light count
                size_t lightIndex = (size_t)lightingUbo.directionalLightCount;
                ++lightingUbo.directionalLightCount;

                // Set the light's direction and color
                lightingUbo.directionalLights[lightIndex].direction = (Vector4)direction;
                lightingUbo.directionalLights[lightIndex].color = color;

                break;
            }
            case LIGHT_TYPE_POINT:
            {
                // Increment the point light count
                size_t lightIndex = (size_t)lightingUbo.pointLightCount;
                ++lightingUbo.pointLightCount;

                // Set the light's position and color
                lightingUbo.pointLights[lightIndex].position = (Vector4)position;
                lightingUbo.pointLights[lightIndex].color = color;

                break;
            }
            }
        }

        static vector<Light*> GetLights() {
            return lights;
        }

        ~Light() {
            for(size_t i = index + 1; i < lights.size(); ++i)
                --lights[i]->index;
            lights.erase(lights.begin() + index);
        }
    private:
        static vector<Light*> lights;
        size_t index;
    };
}