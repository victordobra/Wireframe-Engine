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
    // Testing variables
    Shader* vertShader;
    Shader* fragShader;
    Pipeline* pipeline;
    Material* material;

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
        // Create assets and objects for testing
        Image* diffuseTexture = Asset::GetOrCreateAssetWithLocation<Image>("images/ding.png");
        diffuseTexture->Save("images/ding.png");

        vertShader = new Shader("shaders/VertShader.vert.spv");
        fragShader = new Shader("shaders/FragShader.frag.spv");

        fragShader->AddProperty({ "diffuseColor",     Shader::ShaderProperty::SHADER_PROPERTY_TYPE_COLOR });
        fragShader->AddProperty({ "diffuseTexture",   Shader::ShaderProperty::SHADER_PROPERTY_TYPE_IMAGE });
        fragShader->AddProperty({ "specularExponent", Shader::ShaderProperty::SHADER_PROPERTY_TYPE_FLOAT });
        fragShader->AddProperty({ "specularColor",    Shader::ShaderProperty::SHADER_PROPERTY_TYPE_COLOR });
        fragShader->AddProperty({ "specularTexture",  Shader::ShaderProperty::SHADER_PROPERTY_TYPE_IMAGE });

        vertShader->Save("shaders/VertShader.shader");
        fragShader->Save("shaders/FragShader.shader");
        
        Material* material = new Material(fragShader);

        material->Map();

        Vector4 diffuseColor = { 1.f, 1.f, 1.f, 1.f };
        float32_t specularExponent = 32.f;
        Vector4 specularColor = { 1.f, 1.f, 1.f, 1.f };

        material->SetPropertyValue("diffuseColor",     diffuseColor);
        material->SetPropertyValue("diffuseTexture",   diffuseTexture);
        material->SetPropertyValue("specularExponent", specularExponent);
        material->SetPropertyValue("specularColor",    specularColor);
        material->SetPropertyValue("specularTexture",  diffuseTexture);
        material->Unmap();

        vertShader->Save("shaders/VertShader.shader");
        fragShader->Save("shaders/FragShader.shader");

        material->Save("materials/Default.mat");

        Pipeline::PipelineInfo pipelineInfo;
        Pipeline::PopulatePipelineInfo(pipelineInfo);

        pipelineInfo.pushConstantRanges.resize(1);
        pipelineInfo.pushConstantRanges[0].offset = 0;
        pipelineInfo.pushConstantRanges[0].size = sizeof(ModelRenderer::PushConstant);
        pipelineInfo.pushConstantRanges[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        pipelineInfo.globalBufferSize = sizeof(LightingUbo);

        pipelineInfo.vertexBindings = Model::Vertex::GetBindingDescriptions();
        pipelineInfo.vertexAttributes = Model::Vertex::GetAttributeDescriptions();

        pipelineInfo.shaderStages = { { vertShader, VK_SHADER_STAGE_VERTEX_BIT }, { fragShader, VK_SHADER_STAGE_FRAGMENT_BIT } };

        pipeline = new Pipeline(pipelineInfo);
        pipeline->Save("MainPipeline.pipeline");

        Model* model = new Model("models/Torus.obj");
        model->Save("models/Torus.model");

        Camera* camera = new Camera();
        camera->fov = 60.f;
        camera->SetParent(Node::scene);

        CameraController* controller = new CameraController();
        controller->center = { 0.f, 0.f, -5.f };
        controller->SetParent(camera);

        Body* body = new Body();
        body->position = { 0.f, 0.f, -5.f };
        body->SetParent(Node::scene);
        
        ModelRenderer* modelRenderer = new ModelRenderer();
        modelRenderer->model = model;
        modelRenderer->material = material;
        modelRenderer->SetParent(body);

        Light* light = new Light();
        light->type = Light::LIGHT_TYPE_DIRECTIONAL;
        light->direction = Vector3(1.f, -1.f, -1.f).Normalized();
        light->color = { 1.f, 1.f, 1.f, 1.f };
        light->SetParent(Node::scene);

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