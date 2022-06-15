#pragma once

#include "Asset.hpp"
#include "Core.hpp"
#include "Math/EngineMath.hpp"
#include <typeinfo>
#include <unistd.h>
#include <pthread.h>

namespace mge {
    class Node;

    struct NodeType {
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

        string name{};
        size_t hashCode{};
        vector<Property> properties{};
        Node*(*create)();

        bool8_t operator==(const NodeType& other) const {
            return hashCode == other.hashCode;
        }
        bool8_t operator< (const NodeType& other) const {
            return hashCode < other.hashCode;
        }
        bool8_t operator> (const NodeType& other) const {
            return hashCode > other.hashCode;
        }
        bool8_t operator<=(const NodeType& other) const {
            return hashCode <= other.hashCode;
        }
        bool8_t operator>=(const NodeType& other) const {
            return hashCode >= other.hashCode;
        }
    };

    class Node : public Asset {
    public:
        static const size_t MAX_NODE_TYPE_COUNT = 1024;

        static Node* scene;
        static NodeType nodeTypes[MAX_NODE_TYPE_COUNT];
        static size_t nodeTypeCount;

        string name;
        NodeType* nodeType;

        Node();
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

        /// @brief Runs at the start of the game.
        virtual void Start() { }
        /// @brief Runs at every frame of the game.
        virtual void Update() { }
        /// @brief Runs during every render of the game.
        virtual void Render() { }

        /// @brief Sorts all of the node types. Used at the start of the application.
        static void SortNodeTypes();
        /// @brief Returns the node type with the specified hash code.
        /// @param hashCode The node type's hash code.
        /// @return A pointer to the specific node type, or a nullptr if it doesn't exist.
        static NodeType* GetNodeType(size_t hashCode);

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
}

// Macros
#define MGE_NODE(type)  \
namespace { \
    typedef type T; \
    /* Function that creates a node of the specified type and returns it. */ \
    mge::Node* Create ## type ## Node() { \
        mge::Node* node = dynamic_cast<mge::Node*>(new type()); \
        if(!node) \
            mge::console::OutFatalError("Failed to convert object to node type!", 1); \
        return node; \
    } \
 \
    struct type ## NodeTypeInitializer { \
        type ## NodeTypeInitializer() { \
            mge::NodeType nodeType; \
 \
            nodeType.name = #type; \
            nodeType.hashCode = typeid(type).hash_code(); \
            nodeType.create = Create ## type ## Node; \
 \
            type* ptr = nullptr;

#define MGE_ADD_NODE_PROPERTY(propName, accessInfo) \
            mge::NodeType::Property propName ## Property = mge::NodeType::Property::GetPropertyInfo<decltype(ptr->propName)>(); \
            propName ## Property.offset = (uint64_t)&(ptr->propName); \
            propName ## Property.access = mge::NodeType::Property::PROPERTY_ACCESS_ ## accessInfo; \
            nodeType.properties.push_back(propName ## Property);

#define MGE_END_NODE(type) \
            mge::Node::nodeTypes[mge::Node::nodeTypeCount++] = nodeType; \
        } \
    }; \
 \
    type ## NodeTypeInitializer type ## NodeTypeInit; \
}
