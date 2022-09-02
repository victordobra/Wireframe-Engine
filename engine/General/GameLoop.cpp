#include "GameLoop.hpp"
#include "Input.hpp"
#include "Time.hpp"
#include "ECS/EngineECS.hpp"

#include <pthread.h>
#include <errno.h>

// Testing includes
#include "Assets/Rendering/Pipeline.hpp"
#include "Assets/Rendering/Shader.hpp"
#include "Assets/Rendering/Material.hpp"
#include "Assets/General/Image.hpp"
#include "Components/Lighting/LightTypes.hpp"
#include "Components/Rendering/Camera.hpp"

namespace wfe {
    // Structs
    struct SystemArgs {
        System* system;
        vector<Component*> components;
        System::CallbackType callbackType;
    };

    // Internal helper functions
    static void AddComponentsToVector(vector<Component*>& components, GameObject* gameObject) {
        // Add every component
        for(auto* component : gameObject->GetComponents())
            components.push_back(component);

        // Run this function for every child
        for(auto* child : gameObject->GetChildren())
            AddComponentsToVector(components, child);
    }
    static string ThreadErrorCodeToString(int32_t result) {
        switch (result) {
        case EAGAIN:
            return "EAGAIN";
        case EDEADLK:
            return "EDEADLK";
        case EINVAL:
            return "EINVAL";
        case ENOMEM:
            return "ENOMEM";
        case ESRCH:
            return "ESRCH";
        default:
            return (string)"Unknown: " + ToString(result);
        }
    }

    static void* RunCallbackOnSystem(void* args) {
        SystemArgs* systemArgs = (SystemArgs*)args;
        systemArgs->system->callback(systemArgs->components, systemArgs->callbackType);

        return nullptr;
    }
    static void CreateSystemMap(const vector<Component*>& components, map<System*, vector<Component*>>& systemMap) {
        for(auto* component : components) 
            if(component->system)
                systemMap[component->system].push_back(component);
    }
    static void RunCallbacksOnComponents(const map<System*, vector<Component*>>& systemMap, System::CallbackType callbackType) {
        /*
        vector<pthread_t> threads(systemMap.size());
        vector<SystemArgs> argsVec(systemMap.size());

        // Create every thread
        pthread_t* thread = threads.begin();
        SystemArgs* args = argsVec.begin();
        for(auto& pair : systemMap) {
            *args = { pair.val1, pair.val2, callbackType };
            int32_t result = pthread_create(thread++, nullptr, RunCallbackOnSystem, args++);

            if(result)
                console::OutFatalError((string)"Failed to create thread! Error code: " + ThreadErrorCodeToString(result), 1);
        }

        // Join every thread
        for(auto thread : threads) {
            int32_t result = pthread_join(thread, nullptr);

            if(result)
                console::OutFatalError((string)"Failed to join thread! Error code: " + ThreadErrorCodeToString(result), 1);
        */
        for(auto& pair : systemMap) {
            pair.val1->callback(pair.val2, callbackType);
        }
    }

    // External functions
    void StartGameLoop() {
        // Load the scene
        GameObject::scene = new GameObject();
        GameObject::scene->Load("Scene.gameobj");

        // Create the system map
        vector<Component*> components;
        map<System*, vector<Component*>> systemMap;

        AddComponentsToVector(components, GameObject::scene);
        CreateSystemMap(components, systemMap);

        // Run the start callback for every component
        RunCallbacksOnComponents(systemMap, System::CALLBACK_TYPE_START);
    }
    void UpdateGameLoop() {
        // Start the delta time timer and update the input
        UpdateInputValues();
        StartUpdateTimer();

        // Create the system map
        vector<Component*> components;
        map<System*, vector<Component*>> systemMap;

        AddComponentsToVector(components, GameObject::scene);
        CreateSystemMap(components, systemMap);

        // Run all callbacks in order
        RunCallbacksOnComponents(systemMap, System::CALLBACK_TYPE_UPDATE);
        RunCallbacksOnComponents(systemMap, System::CALLBACK_TYPE_BEFORE_RENDER);
        RunCallbacksOnComponents(systemMap, System::CALLBACK_TYPE_RENDER);

        // Stop the delta time timer
        StopUpdateTimer();
    }
}