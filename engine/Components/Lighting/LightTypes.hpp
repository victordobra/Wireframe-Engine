#pragma once

#include "Light.hpp"

namespace wfe {
    class AmbientLight : public Light { 
    public:
        Color32f ambientColor{};

        void WriteToLightingUbo(LightingUbo& ubo) override { 
            ubo.ambientLightColor = { ambientColor.r, ambientColor.g, ambientColor.b, ambientColor.a };
        }
    };
    class DirectionalLight : public Light {
    public:
        Color32f lightColor{};

        void WriteToLightingUbo(LightingUbo& ubo) override { 
            const Transform& transform = GetGameObject()->transform;

            // Create the info about the directional light
            LightingUbo::DirectionalLight directionalLightInfo;
            directionalLightInfo.direction = transform.GetRotationMatrix() * Vector4(0.f, 0.f, -1.f, 1.f);
            directionalLightInfo.color = { lightColor.r, lightColor.g, lightColor.b, lightColor.a };

            // Add it to the array
            ubo.directionalLights[ubo.directionalLightCount++] = directionalLightInfo;
        }
    };
    class PointLight : public Light {
    public:
        Color32f lightColor{};

        void WriteToLightingUbo(LightingUbo& ubo) override {
            const Transform& transform = GetGameObject()->transform;

            // Create the info about the point light
            LightingUbo::PointLight pointLightInfo;
            pointLightInfo.position = transform.position;
            pointLightInfo.color = { lightColor.r, lightColor.g, lightColor.b, lightColor.a };

            // Add it to the array
            ubo.pointLights[ubo.pointLightCount++] = pointLightInfo;
        }
    };
}