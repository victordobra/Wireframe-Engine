#include "Component.hpp"
#include "GameObject.hpp"

namespace wfe {
    // Static variables
    constinit ComponentType Component::componentTypes[MAX_COMPONENT_TYPE_COUNT];
    constinit size_t Component::componentTypeCount = 0;

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
            str.resize(length);

            input.ReadBuffer((char_t*)str.c_str(), length * sizeof(char_t));
            str[length] = 0;

            Asset*& ptr = *(Asset**)address;
            ptr = nullptr;
            for(auto* asset : Asset::GetAssets())
                if(asset->location == str) {
                    ptr = asset;
                    break;
                }

            if(!ptr) {
                ptr = Asset::GetAssetPtrType(prop.hashCode)->create();
                ptr->Load(str);
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
            auto str = ptr->location;

            uint64_t length = str.length();
            output.WriteBuffer((char_t*)&length, sizeof(uint64_t));
            output.WriteBuffer((char_t*)str.c_str(), length * sizeof(char_t));

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
    void Component::SortComponentTypes() {
        // Sort using the set class
        set<ComponentType> componentTypeSet;

        ComponentType* ptr = componentTypes;
        for(size_t i = 0; i < componentTypeCount; ++i)
            componentTypeSet.insert(*ptr++);
        
        ptr = componentTypes;
        for(const auto& componentType : componentTypeSet)
            *ptr++ = componentType;
        
        // Try to find asset types for every single property
        for(size_t i = 0; i < componentTypeCount; ++i)
            for(auto& property : componentTypes[i].properties)
                if(Asset::GetAssetPtrType(property.hashCode))
                    property.type = ComponentType::Property::PROPERTY_TYPE_ASSET_PTR;
    }
    ComponentType* Component::GetComponentType(uint64_t hashCode) {
        // Binary search for the type
        size_t pos = 0, step = MAX_COMPONENT_TYPE_COUNT >> 1;

        while(step) {
            if(pos + step < componentTypeCount && componentTypes[pos + step].hashCode <= hashCode)
                pos += step;
            step >>= 1;
        }

        if(componentTypes[pos].hashCode == hashCode)
            return componentTypes + pos;
        return nullptr;
    }
}