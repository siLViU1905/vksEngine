#ifndef VERTEX_H
#define VERTEX_H
#include <glm/glm.hpp>
#include <vulkan/vulkan_raii.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

struct Vertex
{
   Vertex();

   Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texCoords);

   Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texCoords, const glm::vec3& tangent, const glm::vec3& biTangent);

   static vk::VertexInputBindingDescription getBindingDescription();

   static std::array<vk::VertexInputAttributeDescription, 5> getAttributeDescription();

   glm::vec3 m_Position;

   glm::vec3 m_Normal;

   glm::vec2 m_TexCoords;

   glm::vec3 m_Tangent;

   glm::vec3 m_Bitangent;

   bool operator==(const Vertex &other) const;
};

namespace std
{
   template<>
   struct hash<Vertex>
   {
      size_t operator()(Vertex const& vertex) const noexcept
      {
         return ((hash<glm::vec3>()(vertex.m_Position) ^
                (hash<glm::vec2>()(vertex.m_TexCoords) << 1)) >> 1);
      }
   };
}

#endif //VERTEX_H
