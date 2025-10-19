#include "Mesh.h"

#include <filesystem>
#include <print>
#include <ranges>

#include "../timer/Clock.h"

namespace vks_engine
{
    Mesh::Mesh(): m_Position(0.f), m_Scale(1.f),
                  m_Rotation(0.f), m_Model(1.f),
                  m_HasTangentsAndBitangents(true),
                  m_Instances(1),
                  m_VertexBuffer(nullptr), m_IndexBuffer(nullptr),
                  m_VertexBufferMemory(nullptr), m_IndexBufferMemory(nullptr),
                  m_ID(std::numeric_limits<uint32_t>::max()),
                  m_Color(0.7f),
                  m_Type(MeshType::NaN)
    {
    }

    Mesh::Mesh(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale, uint32_t instances,
               MeshType type)
        : m_Position(position),
          m_Rotation(rotation),
          m_Scale(scale),
          m_HasTangentsAndBitangents(true),
          m_Instances(instances),
          m_VertexBuffer(nullptr), m_IndexBuffer(nullptr),
          m_VertexBufferMemory(nullptr), m_IndexBufferMemory(nullptr),
          m_ID(std::numeric_limits<uint32_t>::max()),
          m_Color(0.7f),
          m_Type(type)
    {
        updateModel();
    }

    Mesh::Mesh(Mesh &&other) noexcept
        : m_Position(other.m_Position),
          m_Rotation(other.m_Rotation),
          m_Scale(other.m_Scale),
          m_Color(other.m_Color),
          m_Model(other.m_Model),
          m_Vertices(std::move(other.m_Vertices)),
          m_Indices(std::move(other.m_Indices)),
          m_Instances(other.m_Instances),

          m_VertexBuffer(std::move(other.m_VertexBuffer)),
          m_VertexBufferMemory(std::move(other.m_VertexBufferMemory)),
          m_IndexBuffer(std::move(other.m_IndexBuffer)),
          m_IndexBufferMemory(std::move(other.m_IndexBufferMemory)),
          m_DescriptorSets(std::move(other.m_DescriptorSets)),

          m_ID(other.m_ID),
          texturePaths(std::move(other.texturePaths)),
          m_Textures(std::move(other.m_Textures)),
          m_HasTangentsAndBitangents(other.m_HasTangentsAndBitangents),
          m_Type(other.m_Type)
    {
        other.m_ID = std::numeric_limits<uint32_t>::max();
        other.m_Instances = 0;
    }

    Mesh &Mesh::operator=(Mesh &&other) noexcept
    {
        if (this != &other)
        {
            m_Position = other.m_Position;
            m_Rotation = other.m_Rotation;
            m_Scale = other.m_Scale;
            m_Color = other.m_Color;
            m_Model = other.m_Model;
            m_Instances = other.m_Instances;
            m_ID = other.m_ID;
            m_HasTangentsAndBitangents = other.m_HasTangentsAndBitangents;

            m_Vertices = std::move(other.m_Vertices);
            m_Indices = std::move(other.m_Indices);
            texturePaths = std::move(other.texturePaths);
            m_Textures = std::move(other.m_Textures);

            m_VertexBuffer = std::move(other.m_VertexBuffer);
            m_VertexBufferMemory = std::move(other.m_VertexBufferMemory);
            m_IndexBuffer = std::move(other.m_IndexBuffer);
            m_IndexBufferMemory = std::move(other.m_IndexBufferMemory);
            m_DescriptorSets = std::move(other.m_DescriptorSets);

            m_Type = other.m_Type;

            other.m_ID = std::numeric_limits<uint32_t>::max();
            other.m_Instances = 0;
        }

        return *this;
    }

    std::pair<std::string, bool> Mesh::load(std::string_view path)
    {
        Clock clock;

        findDirectory(path);

        Assimp::Importer import;

        const aiScene *scene = import.ReadFile(
            path.data(),
            aiProcess_Triangulate |
            aiProcess_FlipUVs |
            aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
            !scene->mRootNode)
            return {"Failed to load model, file format not supported\n", false};

        processNode(scene->mRootNode, scene);

        uniqueVertices.clear();

        if (!m_HasTangentsAndBitangents)
            calculateTangentsAndBitangents();

        auto timeTook = clock.getElapsedTime<float, TimeType::Seconds>();

        return {std::format("Model successfully loaded and it took {:.3f} seconds\n", timeTook), true};
    }

    void Mesh::move(const glm::vec3 &offset)
    {
        m_Position += offset;

        updateModel();
    }

    void Mesh::rotate(const glm::vec3 &offset)
    {
        m_Rotation += offset;

        updateModel();
    }

    void Mesh::scale(const glm::vec3 &offset)
    {
        m_Scale += offset;

        updateModel();
    }

    void Mesh::setPosition(const glm::vec3 &position)
    {
        m_Position = position;

        updateModel();
    }

    void Mesh::setRotation(const glm::vec3 &rotation)
    {
        m_Rotation = rotation;

        updateModel();
    }

    void Mesh::setScale(const glm::vec3 &scale)
    {
        m_Scale = scale;

        updateModel();
    }

    void Mesh::setColor(const glm::vec3 &color)
    {
        m_Color = color;
    }

    void Mesh::setInstances(uint32_t instances)
    {
        m_Instances = instances;
    }

    void Mesh::increaseInstances(uint32_t value)
    {
        m_Instances += value;
    }

    void Mesh::decreaseInstances(uint32_t values)
    {
        m_Instances -= values;
    }

    void Mesh::setID(uint32_t id)
    {
        m_ID = id;
    }

    void Mesh::setType(MeshType type)
    {
        m_Type = type;
    }

    Texture &Mesh::getTexture(TextureType type)
    {
        return m_Textures[type];
    }

    void Mesh::processNode(aiNode *node, const aiScene *scene)
    {
        for (uint32_t i = 0; i < node->mNumMeshes; ++i)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];

            processMesh(mesh, scene);
        }

        for (uint32_t i = 0; i < node->mNumChildren; ++i)
            processNode(node->mChildren[i], scene);
    }

    void Mesh::processMesh(aiMesh *mesh, const aiScene *scene)
    {
        for (uint32_t i = 0; i < mesh->mNumFaces; ++i)
        {
            const aiFace &face = mesh->mFaces[i];

            for (uint32_t j = 0; j < face.mNumIndices; ++j)
            {
                uint32_t vertexIndex = face.mIndices[j];

                Vertex vertex;

                vertex.m_Position = glm::vec3(mesh->mVertices[vertexIndex].x,
                                              mesh->mVertices[vertexIndex].y,
                                              mesh->mVertices[vertexIndex].z);

                if (mesh->HasNormals())
                    vertex.m_Normal = glm::vec3(mesh->mNormals[vertexIndex].x,
                                                mesh->mNormals[vertexIndex].y,
                                                mesh->mNormals[vertexIndex].z);


                if (mesh->mTextureCoords[0])
                    vertex.m_TexCoords = glm::vec2(mesh->mTextureCoords[0][vertexIndex].x,
                                                   mesh->mTextureCoords[0][vertexIndex].y);


                if (mesh->HasTangentsAndBitangents())
                {
                    vertex.m_Tangent = glm::vec3(mesh->mTangents[vertexIndex].x,
                                                 mesh->mTangents[vertexIndex].y,
                                                 mesh->mTangents[vertexIndex].z);

                    vertex.m_Bitangent = glm::vec3(mesh->mBitangents[vertexIndex].x,
                                                   mesh->mBitangents[vertexIndex].y,
                                                   mesh->mBitangents[vertexIndex].z);
                } else
                    m_HasTangentsAndBitangents = false;

                if (!uniqueVertices.contains(vertex))
                {
                    uniqueVertices[vertex] = static_cast<uint32_t>(m_Vertices.size());
                    m_Vertices.push_back(vertex);
                }

                m_Indices.push_back(uniqueVertices[vertex]);
            }
        }

        if (mesh->mMaterialIndex >= 0)
        {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

            processMaterial(material, scene);
        }
    }

    void Mesh::processMaterial(const aiMaterial *material, const aiScene *scene)
    {
        for (const auto &textureType: SUPPORTED_TEXTURE_TYPES)
        {
            auto aiTextureType = vks_engine_TextureType_to_aiTextureType(textureType);

            uint32_t textureCount = material->GetTextureCount(aiTextureType);

            if (textureCount)
                std::print("has {} of {} texture\n", textureCount, static_cast<int>(textureType));

            for (uint32_t i = 0; i < textureCount; i++)
            {
                aiString texturePath;

                if (material->GetTexture(aiTextureType, i, &texturePath) == aiReturn_SUCCESS)
                {
                    std::string relativePath(texturePath.C_Str());

                    auto fullPath = m_Directory + '/' + relativePath;

                    texturePaths.emplace(textureType, fullPath);
                }
            }
        }
    }

    void Mesh::findDirectory(std::string_view fullPath)
    {
        std::filesystem::path p(fullPath);

        m_Directory = p.parent_path().string();

        std::ranges::replace(m_Directory, '\\', '/');
    }

    void Mesh::updateModel()
    {
        m_Model = glm::mat4(1.f);

        m_Model = glm::translate(m_Model, m_Position);

        m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.x), glm::vec3(1.f, 0.f, 0.f));

        m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.y), glm::vec3(0.f, 1.f, 0.f));

        m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.z), glm::vec3(0.f, 0.f, 1.f));

        m_Model = glm::scale(m_Model, m_Scale);
    }

    void Mesh::calculateTangentsAndBitangents()
    {
        for (size_t i = 0; i < m_Indices.size(); i += 3)
        {
            Vertex &v0 = m_Vertices[m_Indices[i]];
            Vertex &v1 = m_Vertices[m_Indices[i + 1]];
            Vertex &v2 = m_Vertices[m_Indices[i + 2]];

            glm::vec3 edge1 = v1.m_Position - v0.m_Position;
            glm::vec3 edge2 = v2.m_Position - v0.m_Position;

            glm::vec2 deltaUV1 = v1.m_TexCoords - v0.m_TexCoords;
            glm::vec2 deltaUV2 = v2.m_TexCoords - v0.m_TexCoords;

            float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

            glm::vec3 tangent;
            tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
            tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
            tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

            glm::vec3 bitangent;
            bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
            bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
            bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

            v0.m_Tangent += tangent;
            v1.m_Tangent += tangent;
            v2.m_Tangent += tangent;

            v0.m_Bitangent += bitangent;
            v1.m_Bitangent += bitangent;
            v2.m_Bitangent += bitangent;
        }

        for (auto &vertex: m_Vertices)
        {
            const glm::vec3 &n = vertex.m_Normal;
            const glm::vec3 &t = vertex.m_Tangent;


            vertex.m_Tangent = glm::normalize(t - n * glm::dot(n, t));

            if (glm::dot(glm::cross(n, t), vertex.m_Bitangent) < 0.0f)
                vertex.m_Tangent = vertex.m_Tangent * -1.0f;


            vertex.m_Bitangent = glm::normalize(glm::cross(vertex.m_Normal, vertex.m_Tangent));
        }
    }

    std::string Mesh::loadTextures()
    {
        Clock clock;

        for (const auto &[type, path]: texturePaths)
        {
            if (auto [it, emplaced] = m_Textures.emplace(type, path); emplaced)
            {
                auto &texture = it->second;

                texture.m_Type = type;

                texture.load(path);
            }
        }

        texturePaths.clear();

        auto timeTook = clock.getElapsedTime<float, TimeType::Seconds>();

        return std::format("Loaded {} textures and it took {:.3f} seconds\n", m_Textures.size(), timeTook);
    }

    Mesh Mesh::generateSphere(const glm::vec3 &position, float radius, int slices, int stacks)
    {
        Mesh sphere(position, {}, {radius, radius, radius}, 1, MeshType::SPHERE);

        auto &vertices = sphere.m_Vertices;

        auto &indices = sphere.m_Indices;

        for (int i = 0; i <= stacks; ++i)
        {
            float phi = 3.14f * static_cast<float>(i) / static_cast<float>(stacks);
            float y = radius * std::cos(phi);
            float ringRadius = radius * std::sin(phi);

            for (int j = 0; j <= slices; ++j)
            {
                float theta = 2.0f * 3.14f * static_cast<float>(j) / static_cast<float>(slices);

                Vertex vertex;

                vertex.m_Position.x = ringRadius * std::cos(theta);
                vertex.m_Position.y = y;
                vertex.m_Position.z = ringRadius * std::sin(theta);

                vertex.m_Normal.x = vertex.m_Position.x / radius;
                vertex.m_Normal.y = vertex.m_Position.y / radius;
                vertex.m_Normal.z = vertex.m_Position.z / radius;

                vertex.m_TexCoords.x = static_cast<float>(j) / static_cast<float>(slices);
                vertex.m_TexCoords.y = static_cast<float>(i) / static_cast<float>(stacks);

                vertex.m_Tangent.x = -std::sin(theta);
                vertex.m_Tangent.y = 0.0f;
                vertex.m_Tangent.z = std::cos(theta);

                vertex.m_Bitangent = glm::cross(vertex.m_Normal, vertex.m_Tangent);
                vertex.m_Bitangent = glm::normalize(vertex.m_Bitangent);

                vertices.push_back(vertex);
            }
        }

        for (int i = 0; i < stacks; ++i)
        {
            for (int j = 0; j < slices; ++j)
            {
                int first = i * (slices + 1) + j;
                int second = first + slices + 1;

                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(first + 1);

                indices.push_back(second);
                indices.push_back(second + 1);
                indices.push_back(first + 1);
            }
        }

        return sphere;
    }

    Mesh Mesh::generateCube(const glm::vec3 &position, float size)
    {
        Mesh cube(position, {}, {size, size, size}, 1, MeshType::CUBE);

        auto &vertices = cube.m_Vertices;

        auto &indices = cube.m_Indices;

        float half = size / 2.f;

        vertices.push_back(Vertex(glm::vec3(-half, -half, half), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f),
                                  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
        vertices.push_back(Vertex(glm::vec3(half, -half, half), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f),
                                  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
        vertices.push_back(Vertex(glm::vec3(half, half, half), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f),
                                  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
        vertices.push_back(Vertex(glm::vec3(-half, half, half), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f),
                                  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));


        vertices.push_back(Vertex(glm::vec3(half, -half, -half), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f),
                                  glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
        vertices.push_back(Vertex(glm::vec3(-half, -half, -half), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f),
                                  glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
        vertices.push_back(Vertex(glm::vec3(-half, half, -half), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f),
                                  glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
        vertices.push_back(Vertex(glm::vec3(half, half, -half), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f),
                                  glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));


        vertices.push_back(Vertex(glm::vec3(-half, -half, -half), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f),
                                  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
        vertices.push_back(Vertex(glm::vec3(-half, -half, half), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f),
                                  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
        vertices.push_back(Vertex(glm::vec3(-half, half, half), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f),
                                  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
        vertices.push_back(Vertex(glm::vec3(-half, half, -half), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f),
                                  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));


        vertices.push_back(Vertex(glm::vec3(half, -half, half), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f),
                                  glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
        vertices.push_back(Vertex(glm::vec3(half, -half, -half), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f),
                                  glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
        vertices.push_back(Vertex(glm::vec3(half, half, -half), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f),
                                  glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
        vertices.push_back(Vertex(glm::vec3(half, half, half), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f),
                                  glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));


        vertices.push_back(Vertex(glm::vec3(-half, -half, -half), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f),
                                  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
        vertices.push_back(Vertex(glm::vec3(half, -half, -half), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f),
                                  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
        vertices.push_back(Vertex(glm::vec3(half, -half, half), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f),
                                  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
        vertices.push_back(Vertex(glm::vec3(-half, -half, half), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f),
                                  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));


        vertices.push_back(Vertex(glm::vec3(-half, half, half), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f),
                                  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
        vertices.push_back(Vertex(glm::vec3(half, half, half), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f),
                                  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
        vertices.push_back(Vertex(glm::vec3(half, half, -half), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f),
                                  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
        vertices.push_back(Vertex(glm::vec3(-half, half, -half), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f),
                                  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));


        uint32_t faceIndices[] = {
            0, 1, 2, 2, 3, 0
        };

        for (int i = 0; i < 6; i++)
        {
            uint32_t offset = i * 4;
            for (int j = 0; j < 6; j++)
            {
                indices.push_back(offset + faceIndices[j]);
            }
        }

        return cube;
    }
}
