#include "GameObject.hpp"
#include "imgui.hpp"

namespace wfe {
    // Static variables
    GameObject* GameObject::scene = nullptr;

    // Internal variables
    unordered_map<Quaternion*, Vector3> eulerAngleMap;

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
        vector<Component*> components{ this->components.begin(), this->components.end() };
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

    void GameObject::DrawEditorWindow() {
        // Draw the main transform
        if(ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::InputFloat3("Position", transform.position.elements);

            if(!eulerAngleMap.count(&transform.rotation))
                eulerAngleMap.insert(&transform.rotation, transform.rotation.ToEulerAngles());
            
            if(ImGui::InputFloat3("Rotation", eulerAngleMap[&transform.rotation].elements))
                transform.rotation = Quaternion::EulerAngles(eulerAngleMap[&transform.rotation] * DEG_TO_RAD_MULTIPLIER);
            
            ImGui::InputFloat3("Scale", transform.scale.elements);
        }

        for(uint32_t i = 0; i < components.size(); ++i) {
            Component* component = *(components.begin() + i);

            bool8_t open = ImGui::CollapsingHeader(component->componentType->name.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap);
            ImGui::SameLine(ImGui::GetWindowWidth() - 50);
            if(ImGui::Button("X")) {
                components.erase(component);
                --i;
                continue;
            }

            if(open) {
                for(auto& property : component->componentType->properties) {
                    if(property.access == ComponentType::Property::PROPERTY_ACCESS_PRIVATE)
                        continue;
                    
                    char_t* propertyAddress = (char_t*)component + property.offset;

                    switch(property.type) {
                    case ComponentType::Property::PROPERTY_TYPE_INTEGER:
                    {
                        int32_t integer;

                        switch(property.size) {
                        case sizeof(int8_t):
                            integer = *(int8_t*)propertyAddress;
                            break;
                        case sizeof(int16_t):
                            integer = *(int16_t*)propertyAddress;
                            break;
                        case sizeof(int32_t):
                            integer = *(int32_t*)propertyAddress;
                            break;
                        case sizeof(int64_t):
                            integer = *(int64_t*)propertyAddress;
                            break;
                        }

                        ImGui::InputInt(property.name.c_str(), &integer);

                        switch(property.size) {
                        case sizeof(int8_t):
                            *(int8_t*)propertyAddress = (int8_t)integer;
                            break;
                        case sizeof(int16_t):
                            *(int16_t*)propertyAddress = (int16_t)integer;
                            break;
                        case sizeof(int32_t):
                            *(int32_t*)propertyAddress = (int32_t)integer;
                            break;
                        case sizeof(int64_t):
                            *(int64_t*)propertyAddress = (int64_t)integer;
                            break;
                        }
                    }
                        break;
                    case ComponentType::Property::PROPERTY_TYPE_UNSIGNED:
                    {
                        int32_t integer;

                        switch(property.size) {
                        case sizeof(uint8_t):
                            integer = *(uint8_t*)propertyAddress;
                            break;
                        case sizeof(uint16_t):
                            integer = *(uint16_t*)propertyAddress;
                            break;
                        case sizeof(uint32_t):
                            integer = *(uint32_t*)propertyAddress;
                            break;
                        case sizeof(uint64_t):
                            integer = *(uint64_t*)propertyAddress;
                            break;
                        }

                        ImGui::InputInt(property.name.c_str(), &integer);

                        switch(property.size) {
                        case sizeof(uint8_t):
                            *(uint8_t*)propertyAddress = (uint8_t)integer;
                            break;
                        case sizeof(uint16_t):
                            *(uint16_t*)propertyAddress = (uint16_t)integer;
                            break;
                        case sizeof(uint32_t):
                            *(uint32_t*)propertyAddress = (uint32_t)integer;
                            break;
                        case sizeof(uint64_t):
                            *(uint64_t*)propertyAddress = (uint64_t)integer;
                            break;
                        }
                    }
                        break;
                    case ComponentType::Property::PROPERTY_TYPE_FLOAT:
                        if(property.size == sizeof(float64_t)) {
                            ImGui::InputDouble(property.name.c_str(), (float64_t*)propertyAddress);
                        } else {
                            ImGui::InputFloat(property.name.c_str(), (float32_t*)propertyAddress);
                        }

                        break;
                    case ComponentType::Property::PROPERTY_TYPE_STRING:
                    {
                        string& str = *(string*)propertyAddress;

                        char_t buffer[256];
                        if(ImGui::InputText(property.name.c_str(), buffer, 256))
                            str = buffer;
                    }
                        break;
                    case ComponentType::Property::PROPERTY_TYPE_VEC2:
                    {
                        Vector2& vec2 = *(Vector2*)propertyAddress;

                        ImGui::InputFloat2(property.name.c_str(), vec2.elements);
                    }
                        break;
                    case ComponentType::Property::PROPERTY_TYPE_VEC3:
                    {
                        Vector3& vec3 = *(Vector3*)propertyAddress;

                        ImGui::InputFloat3(property.name.c_str(), vec3.elements);
                    }
                        break;
                    case ComponentType::Property::PROPERTY_TYPE_VEC4:
                    {
                        Vector4& vec4 = *(Vector4*)propertyAddress;

                        ImGui::InputFloat4(property.name.c_str(), vec4.elements);
                    }
                        break;
                    case ComponentType::Property::PROPERTY_TYPE_QUAT:
                    {
                        Quaternion& quat = *(Quaternion*)propertyAddress;

                        if(!eulerAngleMap.count(&quat))
                            eulerAngleMap.insert(&quat, quat.ToEulerAngles());
                        
                        if(ImGui::InputFloat3("Rotation", eulerAngleMap[&quat].elements))
                            quat = Quaternion::EulerAngles(eulerAngleMap[&quat] * DEG_TO_RAD_MULTIPLIER);
                    }
                        break;
                    case ComponentType::Property::PROPERTY_TYPE_MAT4X4:
                    {
                        Matrix4x4 mat4x4 = *(Matrix4x4*)propertyAddress;
                        
                        for(size_t i = 0; i < 4; ++i) {
                            ImGui::InputFloat4((property.name + "[" + ToString(i) + "]").c_str(), mat4x4.mat[i]);
                        }
                    }
                        break;
                    case ComponentType::Property::PROPERTY_TYPE_COLOR:
                    {
                        Color32 color;

                        switch(property.size) {
                        case sizeof(Color8):
                            color.r = (*(Color8*)propertyAddress).r / 255.f;
                            color.g = (*(Color8*)propertyAddress).g / 255.f;
                            color.b = (*(Color8*)propertyAddress).b / 255.f;
                            color.a = (*(Color8*)propertyAddress).a / 255.f;
                            break;
                        case sizeof(Color16):
                            color.r = (*(Color16*)propertyAddress).r / 65535.f;
                            color.g = (*(Color16*)propertyAddress).g / 65535.f;
                            color.b = (*(Color16*)propertyAddress).b / 65535.f;
                            color.a = (*(Color16*)propertyAddress).a / 65535.f;
                            break;
                        case sizeof(Color32):
                            color = *(Color32*)propertyAddress;
                            break;
                        case sizeof(Color64):
                            color.r = (*(Color64*)propertyAddress).r;
                            color.g = (*(Color64*)propertyAddress).g;
                            color.b = (*(Color64*)propertyAddress).b;
                            color.a = (*(Color64*)propertyAddress).a;
                            break;
                        }

                        ImGui::ColorEdit4(property.name.c_str(), (float32_t*)&color, ImGuiColorEditFlags_Float);

                        switch(property.size) {
                        case sizeof(Color8):
                            (*(Color8*)propertyAddress).r = color.r * 255;
                            (*(Color8*)propertyAddress).g = color.g * 255;
                            (*(Color8*)propertyAddress).b = color.b * 255;
                            (*(Color8*)propertyAddress).a = color.a * 255;
                            break;
                        case sizeof(Color16):
                            (*(Color16*)propertyAddress).r = color.r * 65535;
                            (*(Color16*)propertyAddress).g = color.g * 65535;
                            (*(Color16*)propertyAddress).b = color.b * 65535;
                            (*(Color16*)propertyAddress).a = color.a * 65535;
                            break;
                        case sizeof(Color32):
                            color = *(Color32*)propertyAddress;
                            break;
                        case sizeof(Color64):
                            (*(Color64*)propertyAddress).r = color.r;
                            (*(Color64*)propertyAddress).g = color.g;
                            (*(Color64*)propertyAddress).b = color.b;
                            (*(Color64*)propertyAddress).a = color.a;
                            break;
                        }
                    }
                        break;
                    case ComponentType::Property::PROPERTY_TYPE_ASSET_PTR:
                    {
                        Asset*& assetPtr = *(Asset**)propertyAddress;
                        AssetType& assetType = Asset::assetTypes.at(property.size);
                        
                        string assetName = "None";
                        if(assetPtr)
                            assetName = assetPtr->name;
                        
                        assetName += (string)"(" + assetType.name + ")";

                        if(ImGui::BeginCombo(property.name.c_str(), assetName.c_str())) {
                            for(Asset* asset : Asset::GetAssets())
                                if(asset->assetType == &assetType && ImGui::Selectable(asset->name.c_str(), assetPtr == asset)) {
                                    assetPtr = asset;
                                    break;
                                }

                            ImGui::EndCombo();
                        }
                    }
                        break;
                    }
                }
            }
        }

        // Create the add component button
        if(ImGui::Button("Add Component", ImVec2(-.1f, 0.f)))
            ImGui::OpenPopup("Add Component");
        
        if(ImGui::BeginPopup("Add Component")) {
            for(auto& type : Component::componentTypes) {
                if(ImGui::MenuItem(type.val2.name.c_str())) {
                    components.insert_or_assign(type.val2.create(this));
                    break;
                }
            }

            ImGui::EndPopup();
        }
    }

    WFE_ASSET(GameObject)
}