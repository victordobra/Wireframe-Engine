#include "LightingSystem.hpp"
#include "Light.hpp"

namespace wfe {
    // Function declarations
    static void LightingSystemCallback(const vector<Component*>& components, System::CallbackType callbackType);

    // System and other variables
    System lightingSystem{ "LightingSystem", LightingSystemCallback };
    LightingUbo lightingUbo{};

    // Function definitions
    static void LightingSystemCallback(const vector<Component*>& components, System::CallbackType callbackType) {
        if(callbackType != System::CALLBACK_TYPE_BEFORE_RENDER)
            return;
        
        // Reset the UBO's values
        lightingUbo = {};

        // Add all lights to a vector
        vector<Light*> lights;
        for(auto* component : components) {
            Light* light = dynamic_cast<Light*>(component);
            if(light)
                lights.push_back(light);
        }

        // Update the lighting ubo based on every light
        for(auto* light : lights)
            light->WriteToLightingUbo(lightingUbo);
    }

    // Public functions
    System* GetLightingSystem() {
        return &lightingSystem;
    }
    LightingUbo* GetLightingUbo() {
        return &lightingUbo;
    }
}