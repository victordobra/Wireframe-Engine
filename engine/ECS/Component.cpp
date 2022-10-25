#include "Component.hpp"
#include "GameObject.hpp"

namespace wfe {
    // Static variables
    constinit map<uint64_t, ComponentType> Component::componentTypes;

    // Load and save property functions
    void ComponentType::Property::LoadProperty(Component* component, ComponentType::Property prop, FileInput& input) {
        char_t* address = (char_t*)component;
        address += prop.offset;

        switch (prop.type) {
        case ComponentType::Property::PROPERTY_TYPE_STRING:
        {
            string& str = *(string*)address;

            uint64_t length{};
            input.ReadBuffer((char_t*)&length, sizeof(uint64_t));
            str.resize(length);

            input.ReadBuffer((char_t*)str.c_str(), length * sizeof(char_t));
            str[length] = 0;
            
            break;
        }
        case ComponentType::Property::PROPERTY_TYPE_ASSET_PTR:
        {
            string str{};

            uint64_t length{};
            input.ReadBuffer((char_t*)&length, sizeof(uint64_t));

            if(length) {
                str.resize(length);

                input.ReadBuffer((char_t*)str.c_str(), length * sizeof(char_t));
                str[length] = 0;

                Asset*& ptr = *(Asset**)address;
                ptr = Asset::GetAssetWithLocation(str);

                if(!ptr) {
                    ptr = Asset::assetTypes[prop.size].create();
                    ptr->Load(str);
                }
            } else {
                *(Asset**)address = nullptr;
            }

            break;
        }
        default:
            input.ReadBuffer(address, prop.size);
            break;
        }
    }
    void ComponentType::Property::SaveProperty(Component* component, ComponentType::Property prop, FileOutput& output) {
        char_t* address = (char_t*)component;
        address += prop.offset;

        switch (prop.type) {
        case ComponentType::Property::PROPERTY_TYPE_STRING:
        {
            string& str = *(string*)address;

            uint64_t length = str.length();
            output.WriteBuffer((char_t*)&length, sizeof(uint64_t));
            output.WriteBuffer((char_t*)str.c_str(), length * sizeof(char_t));

            break;
        }
        case ComponentType::Property::PROPERTY_TYPE_ASSET_PTR:
        {
            Asset* ptr = *(Asset**)address;
            if(ptr) {
                auto str = ptr->location;

                uint64_t length = str.length();
                output.WriteBuffer((char_t*)&length, sizeof(uint64_t));
                output.WriteBuffer((char_t*)str.c_str(), length * sizeof(char_t));
            } else {
                const uint64_t zero = 0;
                output.WriteBuffer((char_t*)&zero, sizeof(uint64_t));
            }

            break;
        }
        default:
            output.WriteBuffer(address, prop.size);
            break;
        }
    }

    // Public member functions
    void Component::SetGameObject(GameObject* newGameObject) {
        if(gameObject)
            gameObject->components.erase(this);
        gameObject = newGameObject;
        if(gameObject)
            gameObject->components.insert(this);
    }

    Component::~Component() {
        if(gameObject)
            gameObject->components.erase(this);
    }

    // Private member functions
    void Component::LoadFromStream(FileInput&  input ) {
        // Load the system
        uint64_t systemNameLength = 0;
        string systemName{};

        input.ReadBuffer((char_t*)&systemNameLength, sizeof(uint64_t));
        systemName.resize(systemNameLength);

        input.ReadBuffer((char_t*)systemName.c_str(), systemNameLength * sizeof(char_t));
        systemName[systemNameLength] = 0;

        system = System::GetSystemWithName(systemName);

        // Load every property
        if(componentType)
            for(const auto& property : componentType->properties)
                ComponentType::Property::LoadProperty(this, property, input);
    }
    void Component::SaveToStream  (FileOutput& output) {
        // Save the system's name
        uint64_t systemNameLength = 0;
        string systemName = "";
        if(system) {
            systemNameLength = (uint64_t)system->name.length();
            systemName = system->name;
        }
        output.WriteBuffer((char_t*)&systemNameLength, sizeof(uint64_t));
        output.WriteBuffer((char_t*)systemName.c_str(), systemNameLength * sizeof(char_t));

        // Save every property
        if(componentType)
            for(const auto& property : componentType->properties)
                ComponentType::Property::SaveProperty(this, property, output);
    }

    // Static functions
    void Component::AddAssetTypeProperties() {
        // Put all asset types into a map with their pointer hash codes as keys
        map<uint64_t, AssetType*> assetPtrTypes;
        for(auto& assetType : Asset::assetTypes)
            assetPtrTypes.insert({ assetType.val2.ptrHashCode, &assetType.val2 });

        // Try to find asset types for every single property
        for(auto& componentType : componentTypes)
            for(auto& property : componentType.val2.properties) {
                auto* assetPtrType = assetPtrTypes.find(property.hashCode);
                if(assetPtrType != assetPtrTypes.end()) {
                    property.type = ComponentType::Property::PROPERTY_TYPE_ASSET_PTR;
                    property.size = assetPtrType->val2->hashCode;
                }
            }
    }
}