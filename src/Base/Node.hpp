#pragma once

#include "Asset.hpp"
#include "Core.hpp"
#include "Math/EngineMath.hpp"
#include <typeinfo>
#include <unistd.h>
#include <pthread.h>

namespace mge {
    struct NodeInfo;

    class Node : public Asset {
    public:
        static Node* scene;
        static unordered_map<size_t, NodeInfo> nodeTypes;
        static vector<pthread_t> nodeThreads;

        static void JoinThreads();

        Node() = default;
        Node(const Node&) = delete;
        Node(Node&&) = delete;

        Node& operator=(const Node&) = delete;
        Node& operator=(Node&&) noexcept = delete;

        /// @brief Returns the parent of the node.
        Node* GetParent() const;
        /// @brief Sets the parent of the node to a new value.
        /// @param newParent The new parent.
        void SetParent(Node* newParent);
        /// @brief Returns the children of the object.
        vector<Node*> GetChildren() const;

        // Runs at the start of the game
        virtual void Start() { }
        // Runs at every frame of the game
        virtual void Update() { }
        // Runs during every render of the game
        virtual void Render() { }

        virtual ~Node();
    protected:
        void LoadFromFile(const string& fileLocation) override;
        void SaveToFile  (const string& fileLocation) override;
    private:
        void LoadFromStream(FileInput&  stream);
        void SaveToStream  (FileOutput& stream);

        Node* parent{nullptr};
        vector<Node*> children{};
    };
    
    struct NodeInfo {
        struct Property {
            enum PropertyType : uint32_t {
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
                PROPERTY_TYPE_ASSET_PTR
            };
            enum PropertyAccess : uint32_t {
                PROPERTY_ACCESS_PRIVATE,
                PROPERTY_ACCESS_PUBLIC
            };

            PropertyType type = PROPERTY_TYPE_OTHER;
            PropertyAccess access = PROPERTY_ACCESS_PRIVATE;
            uint64_t size = 0;
            uint64_t offset = 0;

            template<class T>
            static Property GetPropertyInfo() {
                size_t hashCode = typeid(T).hash_code();

                Property prop;
                prop.size = sizeof(T);
                if(hashCode == typeid(sint16_t).hash_code() || hashCode == typeid(sint32_t).hash_code() || hashCode == typeid(sint64_t).hash_code())
                    prop.type = PROPERTY_TYPE_INTEGER;
                else if(hashCode == typeid(uint16_t).hash_code() || hashCode == typeid(uint32_t).hash_code() || hashCode == typeid(uint64_t).hash_code())
                    prop.type = PROPERTY_TYPE_UNSIGNED;
                else if(hashCode == typeid(float32_t).hash_code() || hashCode == typeid(float64_t).hash_code())
                    prop.type = PROPERTY_TYPE_FLOAT;
                else if(hashCode == typeid(string).hash_code())
                    prop.type = PROPERTY_TYPE_STRING;
                else if(hashCode == typeid(Vector2).hash_code())
                    prop.type = PROPERTY_TYPE_VEC2;
                else if(hashCode == typeid(Vector3).hash_code())
                    prop.type = PROPERTY_TYPE_VEC3;
                else if(hashCode == typeid(Vector4).hash_code())
                    prop.type = PROPERTY_TYPE_VEC4;
                else if(hashCode == typeid(Quaternion).hash_code())
                    prop.type = PROPERTY_TYPE_QUAT;
                else if(hashCode == typeid(Matrix4x4).hash_code())
                    prop.type = PROPERTY_TYPE_MAT4X4;
                else if(Asset::GetAssetType(hashCode)) {
                    prop.type = PROPERTY_TYPE_ASSET_PTR;
                    prop.size = hashCode;
                } else 
                    prop.type = PROPERTY_TYPE_OTHER;

                return prop;
            }
            static void LoadProperty(Node* node, Property prop, FileInput& input);
            static void SaveProperty(Node* node, Property prop, FileOutput& output);
        };

        size_t hashCode = 0;
        string name = "";
        vector<Property> properties = {};
        Node*(*create)() = 0;
    };
}

// Macros
#define MGE_NODE(type) \
mge::Node* Create ## type ## Node() { /* Creates the specified node type */ \
    return new type(); \
} \
namespace { \
    void* Create ## type ## Info(void* pParams) { /* Adds the required information to its hash map */ \
        while(!mge::Node::nodeTypes.bucket_count() || !mge::Node::nodeTypes.begin()) \
            sleep(0.05); \
        mge::NodeInfo info; \
        info.hashCode = typeid(type).hash_code(); \
        info.name = # type; \
        info.create = Create ## type ## Node; \
        type* p = nullptr;

#define MGE_ADD_PROPERTY(type, propType, name, accessInfo) \
        mge::NodeInfo::Property prop ## name = mge::NodeInfo::Property::GetPropertyInfo<propType>(); /* Creates a property with the given info */ \
        prop ## name.offset = (mge::size_t)&p->name; \
        prop ## name.access = mge::NodeInfo::Property::PROPERTY_ACCESS_ ## accessInfo; \
        info.properties.push_back(prop ## name);

#define MGE_END_NODE(type) \
        mge::Node::nodeTypes[info.hashCode] = info; \
        return 0; \
    } \
    struct type ## Creator { \
        type ## Creator() { /* Runs the create function on a separate thread */ \
            pthread_t thread; \
            pthread_create(&thread, NULL, Create ## type ## Info, NULL); \
            mge::Node::nodeThreads.push_back(thread); \
        } \
    }; \
    type ## Creator creator ## type; \
}
