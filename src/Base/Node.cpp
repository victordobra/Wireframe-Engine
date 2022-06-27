#include "Node.hpp"
#include "Math/EngineMath.hpp"

namespace mge {
    // Variables
    Node* Node::scene = nullptr;
    constinit NodeType Node::nodeTypes[MAX_NODE_TYPE_COUNT];
    constinit size_t Node::nodeTypeCount = 0;

    // Member functions
    Node::Node() {
        size_t hashCode = typeid(*this).hash_code();
        nodeType = GetNodeType(hashCode);
    }

    void Node::LoadFromFile(const string& fileLocation) {
        // Open a file stream, load the object from the file and then close the stream
        FileInput input(fileLocation, StreamType::BINARY);

        LoadFromStream(input);

        input.Close();
    }
    void Node::SaveToFile(const string& fileLocation) {
        // Open a file stream, save the object to the file and then close the stream
        FileOutput output(fileLocation, StreamType::BINARY);

        SaveToStream(output);

        output.Close();
    }

    void Node::LoadFromStream(FileInput& stream) {
        // Load the node's name
        uint64_t nameLength{};
        stream.ReadBuffer((char_t*)&nameLength, sizeof(uint64_t));
        name.resize(nameLength);

        stream.ReadBuffer((char_t*)name.c_str(), nameLength * sizeof(char_t));
        name[nameLength] = 0;

        // Load every single property
        for(const auto& property : nodeType->properties)
            NodeType::Property::LoadProperty(this, property, stream);

        // Load the number of children
        uint64_t childCount;
        stream.ReadBuffer((char_t*)&childCount, sizeof(uint64_t));

        // Create every child
        for(size_t i = 0; i < childCount; ++i) {
            size_t hashCode;
            stream.ReadBuffer((char_t*)&hashCode, sizeof(size_t));
            
            Node* child = GetNodeType(hashCode)->create();
            child->SetParent(this);
            child->LoadFromStream(stream);
        }
    }
    void Node::SaveToStream(FileOutput& stream) {
        // Save the node's name
        uint64_t nameLength = name.length();
        stream.WriteBuffer((char_t*)&nameLength, sizeof(uint64_t));
        stream.WriteBuffer((char_t*)name.c_str(), nameLength * sizeof(char_t));

        // Save every single property
        for(const auto& property : nodeType->properties)
            NodeType::Property::SaveProperty(this, property, stream);

        // Save the number of children
        uint64_t childCount = children.size();
        stream.WriteBuffer((char_t*)&childCount, sizeof(uint64_t));

        // Save every child
        for(size_t i = 0; i < childCount; ++i) 
            if(children[i]) {
                Node& ref = *children[i];
                size_t hashCode = typeid(ref).hash_code();
                stream.WriteBuffer((char_t*)&hashCode, sizeof(size_t));
                
                children[i]->SaveToStream(stream);
            }
    }

    Node* Node::GetParent() const {
        return parent;
    }
    void Node::SetParent(Node* newParent) {
        // Remove itself from its parent's list
        if(parent != nullptr)
            for(size_t i = 0; i < parent->children.size(); i++)
                if(parent->children[i] == this) {
                    parent->children.erase(parent->children.begin() + i);
                    break;
                }
        
        // Set the new parent and add itself to its list
        parent = newParent;
        parent->children.push_back(this);
    }
    vector<Node*> Node::GetChildren() const {
        return children;
    }

    Node::~Node() {
        // Remove itself from its parent's list
        if(parent != nullptr)
            for(size_t i = 0; i < parent->children.size(); i++)
                if(parent->children[i] == this) {
                    parent->children.erase(parent->children.begin() + i);
                    break;
                }
        
        // Delete all of its children
        size_t childCount = children.size();
        for(size_t i = 0; i < childCount; i++)
            delete children[0];
    }

    // Load and save property functions
    void NodeType::Property::LoadProperty(Node* node, NodeType::Property prop, FileInput& input) {
        char_t* address = (char_t*)node;
        address += prop.offset;

        switch (prop.type) {
        case NodeType::Property::PROPERTY_TYPE_STRING:
        {
            string& str = *(string*)address;

            uint64_t length{};
            input.ReadBuffer((char_t*)&length, sizeof(uint64_t));
            str.resize(length);

            input.ReadBuffer((char_t*)str.c_str(), length * sizeof(char_t));
            str[length] = 0;
            
            break;
        }
        case NodeType::Property::PROPERTY_TYPE_ASSET_PTR:
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
    void NodeType::Property::SaveProperty(Node* node, NodeType::Property prop, FileOutput& output) {
        char_t* address = (char_t*)node;
        address += prop.offset;

        switch (prop.type) {
        case NodeType::Property::PROPERTY_TYPE_STRING:
        {
            string& str = *(string*)address;

            uint64_t length = str.length();
            output.WriteBuffer((char_t*)&length, sizeof(uint64_t));
            output.WriteBuffer((char_t*)str.c_str(), length * sizeof(char_t));

            break;
        }
        case NodeType::Property::PROPERTY_TYPE_ASSET_PTR:
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

    void Node::SortNodeTypes() { 
        // Sort the list of nodes by using the set class
        set<NodeType> typeSet;
        
        for(size_t i = 0; i < nodeTypeCount; ++i)
            typeSet.insert(nodeTypes[i]);
        
        size_t ind = 0;
        for(const auto& type : typeSet)
            nodeTypes[ind++] = type;
        
        // Try to find asset types for every single property
        for(size_t i = 0; i < nodeTypeCount; ++i)
            for(auto& property : nodeTypes[i].properties)
                if(Asset::GetAssetPtrType(property.hashCode))
                    property.type = NodeType::Property::PROPERTY_TYPE_ASSET_PTR;
    }
    NodeType* Node::GetNodeType(size_t hashCode) { 
        // Binary search for it
        size_t pos = 0, step = MAX_NODE_TYPE_COUNT >> 1;

        while(step) {
            if(pos + step < nodeTypeCount && nodeTypes[pos + step].hashCode <= hashCode)
                pos += step;
            step >>= 1;
        }

        if(nodeTypes[pos].hashCode == hashCode)
            return nodeTypes + pos;
        return nullptr;
    }

    MGE_ASSET(Node)

    MGE_NODE(Node)
    MGE_END_NODE(Node)
}