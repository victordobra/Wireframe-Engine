#include "Model.hpp"
#include <string.h>

namespace mge {
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
        vector<VkVertexInputAttributeDescription> vertexAttributes(3);

        vertexAttributes[0] = { 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position) };
		vertexAttributes[1] = { 1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal)   };
		vertexAttributes[2] = { 2, 0, VK_FORMAT_R32G32_SFLOAT,    offsetof(Vertex, uvCoord)  };

        return vertexAttributes;
    }

    template<>
    void GetHashCode(Model::Vertex vert, size_t& hash) {
        // Combine the hashes of the position, the UV coordinate and the normal
        GetHashCode(vert.position, hash);
        CombineHash(hash, vert.uvCoord);
        CombineHash(hash, vert.normal);
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
        // Open the file stream
        FileInput input((string)ASSET_PATH + fileLocation, StreamType::AT_THE_END);
        size_t fileLength = input.Tell();
        input.Seek(0);

        vector<Vector3> positions{};
        vector<Vector2> uvCoords{};
        vector<Vector3> normals{};

        unordered_map<Vertex, uint32_t> map{};
        vector<Vertex> vertices;
        vector<uint32_t> indices;

        string line;
        // Process every line of the file
        while(!input.IsAtTheEnd()) {
            line = "";
            input.ReadLine(line, 100001);

            // If the line begins with #, it is a comment, skip it
            if(line[0] == '#')
                continue;

            char_t* p = strchr(line.c_str(), ' ');
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
                Vector3 dif1 = verticesArr[1].position - verticesArr[0].position;
                Vector3 dif2 = verticesArr[2].position - verticesArr[0].position;

                if(verticesArr[0].normal.Dot(dif1.Cross(dif2)) > 0.f) {
                    Vertex aux = verticesArr[1];
                    verticesArr[1] = verticesArr[2];
                    verticesArr[2] = aux;
                }

                // Add every vertex or its index to the array
                for(size_t i = 0; i < 3; ++i) {
                    if(map.count(verticesArr[i])) {
                        indices.push_back(map[verticesArr[i]]);
                    } else {
                        size_t ind = vertices.size();
                        map.insert(verticesArr[i], ind);
                        vertices.push_back(verticesArr[i]);
                        indices.push_back(ind);
                    }
                }
            }
        }

        // Create the vertex and index buffers
        CreateVertexBuffer(vertices);
        CreateIndexBuffer(indices);
    }

    void Model::LoadFromFile(const string& fileLocation) {
        FileInput input(fileLocation, StreamType::BINARY);

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
    void Model::SaveToFile  (const string& fileLocation) {
        FileOutput output(fileLocation, StreamType::BINARY);

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

        output.Close();
    }

    Model::~Model() {
        delete vertexBuffer;
        delete indexBuffer;
    }

    MGE_ASSET(Model)
}