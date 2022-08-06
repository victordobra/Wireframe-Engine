#include "GameObject.hpp"

namespace wfe {
    // Static variables
    GameObject* GameObject::scene = nullptr;

    // Public member functions
    GameObject::GameObject(GameObject* parent) : parent(parent) {
        if(parent)
            parent->children.insert(this);
    }

    void GameObject::SetParent(GameObject* newParent) {
        if(parent)
            parent->children.erase(this);
        parent = newParent;
        if(parent)
            parent->children.insert(this);
    }

    void GameObject::LoadFromFile(const string& fileLocation) { 
        // Create the stream
        FileInput input(fileLocation, STREAM_TYPE_BINARY);
        if(!input)
            console::OutFatalError((string)"Failed to open file! Given path: " + fileLocation, 1);

        // Load from the stream and then close it
        LoadFromStream(input);

        input.Close();
    }
    void GameObject::SaveToFile  (const string& fileLocation) { 
        // Create the stream
        FileOutput output(fileLocation, STREAM_TYPE_BINARY);
        if(!output)
            console::OutFatalError((string)"Failed to open file! Given path: " + fileLocation, 1);

        // Save to the stream and then close it
        SaveToStream(output);

        output.Close();
    }

    GameObject::~GameObject() {
        // Erase fropm the parent's list
        if(parent)
            parent->children.erase(this);

        // Delete every child
        vector<GameObject*> children{ this->children.begin(), this->children.end() };
        for(auto* child : children)
            delete child;
        
        // Delete every component
        for(auto* component : components)
            delete component;
    }

    // Private member functions
    void GameObject::LoadFromStream(FileInput&  input ) {
        // Load the object's name
        uint64_t nameLength{};

        input.ReadBuffer((char_t*)&nameLength, sizeof(uint64_t));
        name.resize(nameLength);

        input.ReadBuffer((char_t*)name.c_str(), nameLength * sizeof(char_t));
        name[nameLength] = 0;

        // Load the object's transform
        input.ReadBuffer((char_t*)&transform, sizeof(Transform));

        // Load every component
        uint64_t componentCount{};
        input.ReadBuffer((char_t*)&componentCount, sizeof(uint64_t));

        for(uint64_t i = 0; i < componentCount; ++i) {
            uint64_t hashCode{};
            input.ReadBuffer((char_t*)&hashCode, sizeof(uint64_t));

            auto* componentType = Component::componentTypes.find(hashCode);
            assert((componentType != Component::componentTypes.end()) && "Invalid type read from a file!");

            Component* component = componentType->val2.create(this);

            component->LoadFromStream(input);
        }

        // Load every child
        uint64_t childCount{};
        input.ReadBuffer((char_t*)&childCount, sizeof(uint64_t));

        for(uint64_t i = 0; i < childCount; ++i) {
            GameObject* child = new GameObject(this);
            child->LoadFromStream(input);
        }
    }
    void GameObject::SaveToStream  (FileOutput& output) {
        // Save the object's name
        uint64_t nameLength = (uint64_t)name.length();
        output.WriteBuffer((char_t*)&nameLength, sizeof(uint64_t));
        output.WriteBuffer((char_t*)name.c_str(), nameLength * sizeof(char_t));

        // Save the object's transform
        output.WriteBuffer((char_t*)&transform, sizeof(Transform));

        // Save every component
        uint64_t componentCount = (uint64_t)components.size();
        output.WriteBuffer((char_t*)&componentCount, sizeof(uint64_t));

        for(auto* component : components) {
            output.WriteBuffer((char_t*)&component->componentType->hashCode, sizeof(uint64_t));
            component->SaveToStream(output);
        }

        // Save every child
        uint64_t childCount = (uint64_t)children.size();
        output.WriteBuffer((char_t*)&childCount, sizeof(uint64_t));

        for(auto* child : children)
            child->SaveToStream(output);
    }
}