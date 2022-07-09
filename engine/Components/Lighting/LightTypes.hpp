#pragma once

#include "Light.hpp"

namespace wfe {
    class AmbientLight : public Light { 
    public:
        Vector4 ambientColor{};

        void WriteToLightingUbo(LightingUbo& ubo) override { 
            ubo.ambientLightColor = ambientColor;
        }
    };
    class DirectionalLight : public Light {
    public:
        Vector4 lightColor{};

        void WriteToLightingUbo(LightingUbo& ubo) override { 
            const Transform& transform = GetGameObject()->transform;

            // Create the info about the directional light
            LightingUbo::DirectionalLight directionalLightInfo;
            directionalLightInfo.direction = transform.GetRotationMatrix() * Vector4(0.f, 0.f, -1.f, 1.f);
            directionalLightInfo.color = lightColor;

            // Add it to the array
            ubo.directionalLights[ubo.directionalLightCount++] = directionalLightInfo;
        }
    };
    class PointLight : public Light {
    public:
        Vector4 lightColor{};

        void WriteToLightingUbo(LightingUbo& ubo) override {
            const Transform& transform = GetGameObject()->transform;

            // Create the info about the point light
            LightingUbo::PointLight pointLightInfo;
            pointLightInfo.position = transform.position;
            pointLightInfo.color = lightColor;

            // Add it to the array
            ubo.pointLights[ubo.pointLightCount++] = pointLightInfo;
        }
    };
}