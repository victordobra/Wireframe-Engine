#include "Model.hpp"
#include "WireframeEngineEditor.hpp"
#include <string.h>

namespace wfe {
    // Vertex functions
    vector<VkVertexInputBindingDescription>   Model::Vertex::GetBindingDescriptions() { 
        // Create the vector of vertex bindings
        vector<VkVertexInputBindingDescription> vertexBindings(1);

        vertexBindings[0].binding = 0;
        vertexBindings[0].stride = sizeof(Vertex);
        vertexBindings[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return vertexBindings;
    }
    vector<VkVertexInputAttributeDescription> Model::Vertex::GetAttributeDescriptions() { 
        // Create the vector of vertex attributes
        vector<VkVertexInputAttributeDescription> vertexAttributes(5);

        vertexAttributes[0] = { 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position)  };
		vertexAttributes[2] = { 1, 0, VK_FORMAT_R32G32_SFLOAT,    offsetof(Vertex, uvCoord)   };
        vertexAttributes[1] = { 2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal)    };
        vertexAttributes[3] = { 3, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, tangent)   };
        vertexAttributes[4] = { 4, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, bitangent) };

        return vertexAttributes;
    }

    template<>
    void GetHashCode(Model::Vertex vert, size_t& hash) {
        // Combine the hashes of the position, the UV coordinate and the normal
        GetHashCode(vert.position, hash);
        CombineHash(hash, vert.uvCoord);
        CombineHash(hash, vert.normal);
        CombineHash(hash, vert.tangent);
        CombineHash(hash, vert.bitangent);
    }

    // Internal helper functions
    void Model::CreateVertexBuffer(const vector<Vertex>& vertices ) { 
        // Set some helper variables
        vertexCount = (uint32_t)vertices.size();
		VkDeviceSize bufferSize = sizeof(Vertex) * vertexCount;
		uint32_t vertexSize = sizeof(Vertex);

        // Create the staging buffer and write to it
		Buffer stagingBuffer{ vertexSize, vertexCount, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT };

		stagingBuffer.Map();
		stagingBuffer.WriteToBuffer((void*)vertices.data());
        stagingBuffer.Flush();

        // Create the index buffer and copy it from the staging buffer
		vertexBuffer = new Buffer(vertexSize, vertexCount, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		CopyBuffer(stagingBuffer.GetBuffer(), vertexBuffer->GetBuffer(), bufferSize);
    }
    void Model::CreateIndexBuffer (const vector<uint32_t>& indices) { 
        // Set some helper variables
        indexCount = (uint32_t)indices.size();
		VkDeviceSize bufferSize = sizeof(uint32_t) * indexCount;
		uint32_t indexSize = sizeof(uint32_t);

        // Create the staging buffer and write to it
		Buffer stagingBuffer{ indexSize, indexCount, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT };

		stagingBuffer.Map();
		stagingBuffer.WriteToBuffer((void*)indices.data());
        stagingBuffer.Flush();

        // Create the index buffer and copy it from the staging buffer
		indexBuffer = new Buffer(indexSize, indexCount, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		CopyBuffer(stagingBuffer.GetBuffer(), indexBuffer->GetBuffer(), bufferSize);
    }

    // External functions
    Model::Model(const string& fileLocation) {
        // Get the file location relative to the executable
        string fullLocation;
        if(editor::IsInsideEditor())
            fullLocation = editor::GetWorkspaceDir() + "build/" + WFE_ASSET_LOCATION;
        else
            fullLocation = WFE_ASSET_LOCATION;
        fullLocation += fileLocation;

        // Import the model
        ImportFromFile(fullLocation);
    }

    void Model::LoadFromFile  (const string& fileLocation) {
        FileInput input(fileLocation, STREAM_TYPE_BINARY);

        vector<Vertex> vertices;
        vector<uint32_t> indices;

        // Read all of the vertices from the file
        input.ReadBuffer((char_t*)&vertexCount, sizeof(uint32_t));
        vertices.resize(vertexCount);
        input.ReadBuffer((char_t*)vertices.data(), sizeof(Vertex) * vertexCount);

        // Read all of the indices from the file
        input.ReadBuffer((char_t*)&indexCount, sizeof(uint32_t));
        indices.resize(indexCount);
        input.ReadBuffer((char_t*)indices.data(), sizeof(uint32_t) * indexCount);

        // Create the vertex and index buffers
        CreateVertexBuffer(vertices);
        CreateIndexBuffer(indices);

        input.Close();
    }
    void Model::SaveToFile    (const string& fileLocation) {
        FileOutput output(fileLocation, STREAM_TYPE_BINARY);

        if(vertexCount && indexCount) {
            // Create a vertex staging buffer
            VkDeviceSize vertexBufferSize = sizeof(Vertex) * vertexCount;
            uint32_t vertexSize = sizeof(Vertex);

            Buffer vertexStagingBuffer{ vertexSize, vertexCount, VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT };

            // Create an index staging buffer
            VkDeviceSize indexBufferSize = sizeof(uint32_t) * indexCount;
            uint32_t indexSize = sizeof(uint32_t);
            
            Buffer indexStagingBuffer{ indexSize, indexCount, VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT };

            // Copy to the staging buffers
            CopyBuffer(vertexBuffer->GetBuffer(), vertexStagingBuffer.GetBuffer(), vertexBufferSize);
            CopyBuffer( indexBuffer->GetBuffer(),  indexStagingBuffer.GetBuffer(),  indexBufferSize);

            // Map the staging buffers
            vertexStagingBuffer.Map();
            indexStagingBuffer.Map();

            // Write the vertors' data to the file
            output.WriteBuffer((char_t*)&vertexCount, sizeof(uint32_t));
            output.WriteBuffer((char_t*)vertexStagingBuffer.GetMappedMemory(), (size_t)vertexBufferSize);
            output.WriteBuffer((char_t*)&indexCount, sizeof(uint32_t));
            output.WriteBuffer((char_t*)indexStagingBuffer.GetMappedMemory(), (size_t)indexBufferSize);
        } else {
            output.WriteBuffer((char_t*)&vertexCount, sizeof(uint32_t));
            output.WriteBuffer((char_t*)&indexCount, sizeof(uint32_t));
        }

        output.Close();
    }
    void Model::ImportFromFile(const string& fileLocation) {
        // Open the file stream
        FileInput input(fileLocation);

        vector<Vector3> positions{};
        vector<Vector2> uvCoords{};
        vector<Vector3> normals{};

        unordered_map<Vertex, uint32_t> map{64};
        vector<Vertex> vertices;
        vector<uint32_t> indices;
        vector<Vector3> tangents;
        vector<Vector3> bitangents;
        bool8_t smoothShading = false;

        // Process every line of the file
        while(!input.IsAtTheEnd()) {
            string line = "";
            input.ReadLine(line, 1000001);

            // Only continue if the line isn't a comment
            if(line[0] == '#' || !line.length())
                continue;

            char_t* p = strchr(line.data(), ' ');
            *p++ = 0;

            if(line == "v") {
                // Add the position to the vector of positions
                char_t* y = strchr(p, ' ');
                *y++ = 0;
                char_t* z = strchr(y, ' ');
                *z++ = 0;

                Vector3 vec{ stof(p), stof(y), stof(z) };
                positions.push_back(vec);
            } else if(line == "vt") {
                // Add the UV coordinate to the vector of UV coordinates
                char_t* y = strchr(p, ' ');
                *y++ = 0;

                Vector2 vec{ stof(p), stof(y) };
                uvCoords.push_back(vec);
            } else if(line == "vn") {
                // Add the normal to the vector of normals
                char_t* y = strchr(p, ' ');
                *y++ = 0;
                char_t* z = strchr(y, ' ');
                *z++ = 0;

                Vector3 vec{ stof(p), stof(y), stof(z) };
                normals.push_back(vec);
            } else if(line == "s") {
                uint32_t result = stoul(p);
                smoothShading = result;
            } else if(line == "f") {
                // Split the string into 3 strings for every vertex
                char_t* vertStr[3];

                vertStr[0] = p;
                for(size_t i = 1; i < 3; ++i) {
                    vertStr[i] = strchr(vertStr[i - 1], ' ');
                    *vertStr[i]++ = 0;
                }

                // Generate the vertex from each string
                Vertex verticesArr[3];
                for(size_t i = 0; i < 3; ++i) {
                    char_t* posStr = vertStr[i];
                    char_t* uvStr = strchr(posStr, '/');
                    *uvStr++ = 0;
                    char_t* normStr = strchr(uvStr, '/');
                    *normStr++ = 0;

                    verticesArr[i] = { positions[stoul(posStr) - 1], uvCoords[stoul(uvStr) - 1], normals[stoul(normStr) - 1] };
                }

                // Check if the vertices are ordered correctly; if not, swap the second and third
                Vector3 edge1 = verticesArr[1].position - verticesArr[0].position;
                Vector3 edge2 = verticesArr[2].position - verticesArr[0].position;
                Vector3 averageNormal = (verticesArr[0].normal + verticesArr[1].normal + verticesArr[2].normal) / 3.f;

                if(averageNormal.Dot(edge1.Cross(edge2)) > 0.f) {
                    Vertex auxVert = verticesArr[1];
                    verticesArr[1] = verticesArr[2];
                    verticesArr[2] = auxVert;

                    Vector3 auxEdge = edge1;
                    edge1 = edge2;
                    edge2 = auxEdge;
                }

                // Compute the tangent for the face
                Vector2 deltaUV1 = verticesArr[1].uvCoord - verticesArr[0].uvCoord;
                Vector2 deltaUV2 = verticesArr[2].uvCoord - verticesArr[0].uvCoord;

                float32_t fc = 1.f / (deltaUV1.u * deltaUV2.v - deltaUV1.v * deltaUV2.u);

                Vector3 tangent{ fc * (deltaUV2.v * edge1.x - deltaUV1.v * edge2.x),
                                 fc * (deltaUV2.v * edge1.y - deltaUV1.v * edge2.y),
                                 fc * (deltaUV2.v * edge1.z - deltaUV1.v * edge2.z) };
                tangent.Normalize();
                Vector3 bitangent{ fc * (deltaUV1.u * edge2.x - deltaUV2.u * edge1.x),
                                   fc * (deltaUV1.u * edge2.y - deltaUV2.u * edge1.y),
                                   fc * (deltaUV1.u * edge2.z - deltaUV2.u * edge1.z) };
                bitangent.Normalize();
                
                // Compute the tangent and bitangent for every vertex
                Vector3 tangentsArr[3], bitangentsArr[3];

                if(smoothShading) {
                    Vector3 edge3 = verticesArr[1].position - verticesArr[2].position;

                    float32_t angles[3];
                    angles[0] = edge1.Angle(edge2);
                    angles[1] = edge1.Angle(edge3);
                    angles[2] = PI - angles[0] - angles[1];

                    for(size_t i = 0; i < 3; ++i) {
                        Vector3 tangentVert = tangent, bitangentVert = bitangent, normalVert = verticesArr[i].normal;

                        tangentVert -= normalVert * tangentVert.Dot(normalVert);
                        tangentVert.Normalize();
                        bitangentVert -= normalVert * bitangentVert.Dot(normalVert) + tangentVert * bitangentVert.Dot(tangentVert);
                        bitangentVert.Normalize();

                        tangentsArr[i] = tangentVert * angles[i];
                        bitangentsArr[i] = bitangentVert * angles[i];
                    }
                } else {
                    for(size_t i = 0; i < 3; ++i) {
                        Vector3 tangentVert = tangent, bitangentVert = bitangent, normalVert = verticesArr[i].normal;

                        tangentVert -= normalVert * tangentVert.Dot(normalVert);
                        tangentVert.Normalize();
                        bitangentVert -= normalVert * bitangentVert.Dot(normalVert) + tangentVert * bitangentVert.Dot(tangentVert);
                        bitangentVert.Normalize();

                        verticesArr[i].tangent = tangentVert;
                        verticesArr[i].bitangent = bitangentVert;
                    }
                }
                
                // Add every vertex or its index to the array
                for(size_t i = 0; i < 3; ++i) {
                    Vertex vertex = verticesArr[i];

                    if(map.count(vertex)) {
                        size_t ind = map[vertex];

                        indices.push_back(ind);

                        if(smoothShading) {
                            tangents[ind] += tangentsArr[i];
                            bitangents[ind] += bitangentsArr[i];
                        }
                    } else {
                        size_t ind = vertices.size();

                        map.insert(vertex, ind);
                        indices.push_back(ind);

                        vertices.push_back(vertex);
                        if(smoothShading) {
                            tangents.push_back(tangentsArr[i]);
                            bitangents.push_back(bitangentsArr[i]);
                        }
                    }
                }
            }
        }
        if(smoothShading)
            for(size_t i = 0; i < vertices.size(); ++i) {
                vertices[i].tangent = tangents[i].Normalized();
                vertices[i].bitangent = bitangents[i].Normalized();
            }

        // Create the vertex and index buffers
        CreateVertexBuffer(vertices);
        CreateIndexBuffer(indices);
    }

    Model::~Model() {
        delete vertexBuffer;
        delete indexBuffer;
    }

    WFE_ASSET(Model)
}