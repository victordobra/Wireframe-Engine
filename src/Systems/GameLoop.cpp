#include "GameLoop.hpp"
#include "Base/Node.hpp"
#include "Input.hpp"
#include "Time.hpp"
#include <pthread.h>

// Testing includes
#include "Assets/Rendering/Pipeline.hpp"
#include "Assets/Rendering/Shader.hpp"
#include "Assets/Rendering/Material.hpp"
#include "Assets/General/Image.hpp"
#include "Nodes/Rendering/Camera.hpp"
#include "Nodes/Renderers/ModelRenderer.hpp"
#include "Nodes/Controllers/CameraController.hpp"

namespace mge {
    // Internal helper functions
    static void AddChildrenToVector(vector<Node*>& vec, Node* node) {
        for(auto child : node->GetChildren()) {
            vec.push_back(child);
            AddChildrenToVector(vec, child);
        }
    }
    static void* RunStartOnNode(void* pParams) {
        Node* node = (Node*)pParams;
        node->Start();
        return 0;
    }
    static void* RunUpdateOnNode(void* pParams) {
        Node* node = (Node*)pParams;
        node->Update();
        return 0;
    }
    static void* RunRenderOnNode(void* pParams) {
        Node* node = (Node*)pParams;
        node->Render();
        return 0;
    }
    static string ThreadCreateErrorCoreToString(sint32_t result) {
        switch (result) {
        case EAGAIN:
            return "EAGAIN";
        case EINVAL:
            return "EINVAL";
            break;
        case ENOMEM:
            return "ENOMEM";
            break;
        default:
            return (string)"Unknown: " + ToString(result);
            break;
        }
    }
    static string ThreadJoinErrorCoreToString(sint32_t result) {
        switch (result) {
        case ESRCH:
            return "ESRCH";
        case EINVAL:
            return "EINVAL";
        case EDEADLK:
            return "EDEADLK";
        default:
            return (string)"Unknown: " + ToString(result);
        }
    }

    // External functions
    void StartGameLoop() {
        // Load the scene
        Node::scene = new Node();
        Node::scene->Load("Scene.node");

        // Find all nodes reccursively
        vector<Node*> nodes;

        AddChildrenToVector(nodes, Node::scene);

        // Create a vector of threads
        vector<pthread_t> threads(nodes.size());

        for(size_t i = 0; i < threads.size(); ++i) {
            sint32_t result = pthread_create(&threads[i], NULL, RunStartOnNode, nodes[i]);

            if(result) {
                string resultString = ThreadCreateErrorCoreToString(result);
                console::OutFatalError((string)"Failed to create thread! Error code: " + resultString, 1);
            }
        }

        // Join every thread
        for(size_t i = 0; i < threads.size(); ++i) {
            int result = pthread_join(threads[i], NULL);

            if(result) {
                string resultString = ThreadJoinErrorCoreToString(result);
                console::OutFatalError((string)"Failed to join thread! Error code: " + resultString, 1);
            }
        }
    }
    void UpdateGameLoop() {
        // Start the delta time timer and update the input
        UpdateInputValues();
        StartUpdateTimer();

        // Find all nodes reccursively
        vector<Node*> nodes;

        AddChildrenToVector(nodes, Node::scene);

        // Create a vector of threads
        vector<pthread_t> threads(nodes.size());

        for(size_t i = 0; i < threads.size(); ++i) {
            sint32_t result = pthread_create(&threads[i], NULL, RunUpdateOnNode, nodes[i]);

            if(result) {
                string resultString = ThreadCreateErrorCoreToString(result);
                console::OutFatalError((string)"Failed to create thread! Error code: " + resultString, 1);
            }
        }

        // Join every thread
        for(size_t i = 0; i < threads.size(); ++i) {
            sint32_t result = pthread_join(threads[i], NULL);

            if(result) {
                string resultString = ThreadJoinErrorCoreToString(result);
                console::OutFatalError((string)"Failed to join thread! Error code: " + resultString, 1);
            }
        }

        // Recreate the threads for the render functions
        for(size_t i = 0; i < threads.size(); ++i) {
            sint32_t result = pthread_create(&threads[i], NULL, RunRenderOnNode, nodes[i]);

            if(result) {
                string resultString = ThreadCreateErrorCoreToString(result);
                console::OutFatalError((string)"Failed to create thread! Error code: " + resultString, 1);
            }
        }

        // Join every thread again
        for(size_t i = 0; i < threads.size(); ++i) {
            sint32_t result = pthread_join(threads[i], NULL);

            if(result) {
                string resultString = ThreadJoinErrorCoreToString(result);
                console::OutFatalError((string)"Failed to join thread! Error code: " + resultString, 1);
            }
        }

        // Stop the delta time timer
        StopUpdateTimer();
    }
}