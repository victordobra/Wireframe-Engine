#pragma once

#include "Core.hpp"
#include "System.hpp"
#include "Math/EngineMath.hpp"

namespace wfe {
    class Component;
    class GameObject;

    struct ComponentType {
        struct Property {
            typedef enum : uint32_t {
                PROPERTY_TYPE_OTHER,
                PROPERTY_TYPE_INTEGER,
                PROPERTY_TYPE_UNSIGNED,
                PROPERTY_TYPE_FLOAT,
                PROPERTY_TYPE_STRING,
                PROPERTY_TYPE_VECTOR,
                PROPERTY_TYPE_VEC2,
                PROPERTY_TYPE_VEC3,
                PROPERTY_TYPE_VEC4,
                PROPERTY_TYPE_QUAT,
                PROPERTY_TYPE_MAT4X4,
                PROPERTY_TYPE_COLOR,
                PROPERTY_TYPE_COLORF,
                PROPERTY_TYPE_ASSET_PTR
            } PropertyType;
            typedef enum : uint32_t {
                PROPERTY_ACCESS_PRIVATE,
                PROPERTY_ACCESS_PUBLIC
            } PropertyAccess;

            PropertyType type = PROPERTY_TYPE_OTHER;
            PropertyAccess access = PROPERTY_ACCESS_PRIVATE;
            uint64_t hashCode = 0;
            uint64_t size = 0;
            uint64_t offset = 0;

            template<class T>
            static Property GetPropertyInfo() {
                Property prop{};

                prop.hashCode = WFE_TYPE_ID(T).Hash64();
                prop.size = sizeof(T);

                if(prop.hashCode == WFE_TYPE_ID(int16_t).Hash64() || prop.hashCode == WFE_TYPE_ID(int32_t).Hash64() || prop.hashCode == WFE_TYPE_ID(int64_t).Hash64())
                    prop.type = PROPERTY_TYPE_INTEGER;
                else if(prop.hashCode == WFE_TYPE_ID(uint16_t).Hash64() || prop.hashCode == WFE_TYPE_ID(uint32_t).Hash64() || prop.hashCode == WFE_TYPE_ID(uint64_t).Hash64())
                    prop.type = PROPERTY_TYPE_UNSIGNED;
                else if(prop.hashCode == WFE_TYPE_ID(float32_t).Hash64() || prop.hashCode == WFE_TYPE_ID(float64_t).Hash64())
                    prop.type = PROPERTY_TYPE_FLOAT;
                else if(prop.hashCode == WFE_TYPE_ID(string).Hash64())
                    prop.type = PROPERTY_TYPE_STRING;
                else if(prop.hashCode == WFE_TYPE_ID(Vector2).Hash64())
                    prop.type = PROPERTY_TYPE_VEC2;
                else if(prop.hashCode == WFE_TYPE_ID(Vector3).Hash64())
                    prop.type = PROPERTY_TYPE_VEC3;
                else if(prop.hashCode == WFE_TYPE_ID(Vector4).Hash64())
                    prop.type = PROPERTY_TYPE_VEC4;
                else if(prop.hashCode == WFE_TYPE_ID(Quaternion).Hash64())
                    prop.type = PROPERTY_TYPE_QUAT;
                else if(prop.hashCode == WFE_TYPE_ID(Matrix4x4).Hash64())
                    prop.type = PROPERTY_TYPE_MAT4X4;
                else if(prop.hashCode == WFE_TYPE_ID(Color8).Hash64() || prop.hashCode == WFE_TYPE_ID(Color16).Hash64() || prop.hashCode == WFE_TYPE_ID(Color32).Hash64() || prop.hashCode == WFE_TYPE_ID(Color64).Hash64())
                    prop.type = PROPERTY_TYPE_COLOR;
                else if(prop.hashCode == WFE_TYPE_ID(Color32f).Hash64() || prop.hashCode == WFE_TYPE_ID(Color64f).Hash64())
                    prop.type = PROPERTY_TYPE_COLORF;
                else 
                    prop.type = PROPERTY_TYPE_OTHER;

                return prop;
            }
            static void LoadProperty(Component* component, Property prop, FileInput& input);
            static void SaveProperty(Component* component, Property prop, FileOutput& output);
        };

        string name{};
        uint64_t hashCode = 0;
        vector<Property> properties{};
        System*(*defaultSystemCallback)() = 0;
        Component*(*create)(GameObject*) = 0;

        constexpr ComponentType() = default;
        constexpr ComponentType(const ComponentType&) = default;
        constexpr ComponentType(ComponentType&&) noexcept = default;

        ComponentType& operator=(const ComponentType&) = default;
        ComponentType& operator=(ComponentType&&) noexcept = default;

        bool8_t operator==(const ComponentType& other) const {
            return hashCode == other.hashCode;
        }
        bool8_t operator< (const ComponentType& other) const {
            return hashCode < other.hashCode;
        }
        bool8_t operator> (const ComponentType& other) const {
            return hashCode > other.hashCode;
        }
        bool8_t operator<=(const ComponentType& other) const {
            return hashCode <= other.hashCode;
        }
        bool8_t operator>=(const ComponentType& other) const {
            return hashCode >= other.hashCode;
        }

        constexpr ~ComponentType() = default;
    };

    class Component {
    public:
        static const size_t MAX_COMPONENT_TYPE_COUNT = 1024;

        static ComponentType componentTypes[MAX_COMPONENT_TYPE_COUNT];
        static size_t componentTypeCount;

        ComponentType* componentType = nullptr;
        System* system = nullptr;

        Component() = default;
        Component(const Component&) = delete;
        Component(Component&&) noexcept = delete;

        GameObject* GetGameObject() {
            return gameObject;
        }
        void SetGameObject(GameObject* newGameObject);

        static void SortComponentTypes();
        static ComponentType* GetComponentType(uint64_t hashCode);

        Component& operator=(const Component&) = delete;
        Component& operator=(Component&&) noexcept = delete;

        virtual ~Component();
    private:
        void LoadFromStream(FileInput&  input );
        void SaveToStream  (FileOutput& output);

        GameObject* gameObject;

        friend GameObject;
    };
}

#define WFE_COMPONENT(type) \
namespace { \
    /* Create function for creating a component with the specified type */ \
    wfe::Component* Create ## type ## Component(wfe::GameObject* gameObject) { \
        wfe::Component* ptr = dynamic_cast<wfe::Component*>(new type()); \
        assert(ptr && "The given type must be a component type!"); \
        ptr->SetGameObject(gameObject); \
        ptr->componentType = wfe::Component::GetComponentType(WFE_TYPE_ID(type).Hash64()); \
        if(ptr->componentType->defaultSystemCallback) \
            ptr->system = ptr->componentType->defaultSystemCallback(); \
        return ptr; \
    } \
     \
    struct type ## ComponentInfoInitializer { \
        type ## ComponentInfoInitializer() { \
            type* ptr = nullptr; \
             \
            wfe::ComponentType componentType; \
             \
            componentType.name = #type; \
            componentType.hashCode = WFE_TYPE_ID(type).Hash64(); \
            componentType.create = Create ## type ## Component;
#define WFE_COMPONENT_DEFAULT_SYSTEM(defaultComponentSystemCallback) \
            componentType.defaultSystemCallback = defaultComponentSystemCallback;
#define WFE_COMPONENT_PROPERTY(propertyName, propertyAccess) \
            wfe::ComponentType::Property propertyName ## Property = wfe::ComponentType::Property::GetPropertyInfo<decltype(ptr->propertyName)>(); \
            propertyName ## Property.offset = (size_t)&(ptr->propertyName); \
            propertyName ## Property.access = wfe::ComponentType::Property::PROPERTY_ACCESS_ ## propertyAccess; \
            componentType.properties.push_back(propertyName ## Property);
#define WFE_END_COMPONENT(type) \
            wfe::Component::componentTypes[wfe::Component::componentTypeCount++] = componentType; \
        } \
    }; \
    type ## ComponentInfoInitializer type ## ComponentInfoInit{}; \
}