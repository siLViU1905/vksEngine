#ifndef VKSENGINEMESH_H
#define VKSENGINEMESH_H

#include "Vertex.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <vulkan/vulkan_raii.hpp>

#include "Texture.h"

namespace vks_engine
{
    class Mesh
    {
    private:
        static constexpr std::array<aiTextureType, 22> ALL_AI_TEXTURE_TYPES = {
            aiTextureType_DIFFUSE,
            aiTextureType_SPECULAR,
            aiTextureType_AMBIENT,
            aiTextureType_EMISSIVE,
            aiTextureType_HEIGHT,
            aiTextureType_NORMALS,
            aiTextureType_SHININESS,
            aiTextureType_OPACITY,
            aiTextureType_DISPLACEMENT,
            aiTextureType_LIGHTMAP,
            aiTextureType_REFLECTION,
            aiTextureType_BASE_COLOR,
            aiTextureType_NORMAL_CAMERA,
            aiTextureType_EMISSION_COLOR,
            aiTextureType_METALNESS,
            aiTextureType_DIFFUSE_ROUGHNESS,
            aiTextureType_AMBIENT_OCCLUSION,
            aiTextureType_SHEEN,
            aiTextureType_CLEARCOAT,
            aiTextureType_TRANSMISSION,
            aiTextureType_UNKNOWN,
            aiTextureType_NONE
        };

    public:
        Mesh();

        Mesh(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale, uint32_t instances);

        Mesh(Mesh &&mesh) noexcept;

        Mesh &operator=(Mesh &&mesh) noexcept;

        void load(const std::string &path);

        constexpr const auto &getVertices() const { return m_Vertices; }

        constexpr const auto &getIndices() const { return m_Indices; }

        void move(const glm::vec3 &offset);

        void rotate(const glm::vec3 &offset);

        void scale(const glm::vec3 &offset);

        constexpr const glm::vec3 &getPosition() const
        {
            return m_Position;
        }

        void setPosition(const glm::vec3 &position);

        constexpr const glm::vec3 &getRotation() const
        {
            return m_Rotation;
        }

        void setRotation(const glm::vec3 &rotation);

        constexpr const glm::vec3 &getScale() const
        {
            return m_Scale;
        }

        void setScale(const glm::vec3 &scale);

        constexpr const auto &getModel() const { return m_Model; }

        constexpr const glm::vec3 &getColor() const
        {
            return m_Color;
        }

        void setColor(const glm::vec3 &color);

        constexpr uint32_t getInstances() const { return m_Instances; }

        void setInstances(uint32_t instances);

        void increaseInstances(uint32_t value);

        void decreaseInstances(uint32_t value);

        constexpr uint32_t getID() const { return m_ID; }

        void setID(uint32_t id);

        friend class VulkanHandler;

        friend class Scene;

    private:
        glm::vec3 m_Position;

        glm::vec3 m_Rotation;

        glm::vec3 m_Scale;

        glm::mat4 m_Model;

        glm::vec3 m_Color;

        std::vector<Vertex> m_Vertices;

        std::vector<uint32_t> m_Indices;

        std::unordered_map<Vertex, uint32_t> m_UniqueVertices;

        uint32_t m_Instances;

        vk::raii::Buffer m_VertexBuffer;

        vk::raii::DeviceMemory m_VertexBufferMemory;

        vk::raii::Buffer m_IndexBuffer;

        vk::raii::DeviceMemory m_IndexBufferMemory;

        uint32_t m_ID;

        void updateModel();

        void calculateTangentsAndBitangents();

        std::unordered_set<std::string> m_TexturePaths;

        std::vector<Texture> m_Textures;

        bool m_HasTangentsAndBitangents;

        void processNode(aiNode *node, const aiScene *scene);

        void processMesh(aiMesh *mesh, const aiScene *scene);

        void processMaterial(aiMaterial *material, const aiScene *scene);

        void loadTextures();

    public:
        static Mesh generateSphere(const glm::vec3 &position, float radius, int slices = 32, int stacks = 16);
    };
}
#endif //VKSENGINEMESH_H
