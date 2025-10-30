#ifndef VKSENGINE_MESH_H
#define VKSENGINE_MESH_H

#include "../../VKSEngine.h"
#include "../Vertex.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <vulkan/vulkan_raii.hpp>

#include "MeshType.h"
#include "../Material.h"
#include "../texture/Texture.h"

namespace vks_engine
{
    class Mesh
    {
    private:
        static constexpr std::array<TextureType, SUPPORTED_TEXTURE_TYPES_COUNT> SUPPORTED_TEXTURE_TYPES = {
            TextureType::DIFFUSE,
            TextureType::SPECULAR,
            TextureType::NORMALS,
        };

    public:
        Mesh();

        Mesh(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale, uint32_t instances,
             MeshType type);

        Mesh(Mesh &&mesh) noexcept;

        Mesh &operator=(Mesh &&mesh) noexcept;

        std::pair<std::string, bool> load(std::string_view path);

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

        constexpr MeshType getType() const { return m_Type; }

        void setType(MeshType type);

        constexpr const Material& getMaterial() const{return m_Material;}

        constexpr Material& getMaterial() {return m_Material;}

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

        uint32_t m_Instances;

        bool m_HasTangentsAndBitangents;

        vk::raii::Buffer m_VertexBuffer;

        vk::raii::DeviceMemory m_VertexBufferMemory;

        vk::raii::Buffer m_IndexBuffer;

        vk::raii::DeviceMemory m_IndexBufferMemory;

        std::vector<vk::raii::DescriptorSet> m_DescriptorSets;

        uint32_t m_ID;

        MeshType m_Type;

        Material m_Material;

        std::unordered_map<TextureType, Texture> m_Textures;

        std::string m_Directory;

        std::unordered_map<Vertex, uint32_t> uniqueVertices;

        std::unordered_map<TextureType, std::string> texturePaths;

        void updateModel();

        void calculateTangentsAndBitangents();

        void processNode(aiNode *node, const aiScene *scene);

        void processMesh(aiMesh *mesh, const aiScene *scene);

        void processMaterial(const aiMaterial *material, const aiScene *scene);

        void findDirectory(std::string_view fullPath);

        std::string loadTextures();

    public:
        static Mesh generateSphere(const glm::vec3 &position, float radius, int slices = 32, int stacks = 16);

        static Mesh generateCube(const glm::vec3 &position, float size);
    };
}
#endif //VKSENGINE_MESH_H
