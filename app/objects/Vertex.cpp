#include "Vertex.h"

Vertex::Vertex(): m_Position(0.f), m_Normal(1.f), m_TexCoords(0.f), m_Tangent(0.f), m_Bitangent(0.f)
{
}

Vertex::Vertex(const glm::vec3 &position, const glm::vec3 &normal, const glm::vec2 &texCoords): m_Position(position),
    m_Normal(normal),
    m_TexCoords(texCoords),
    m_Tangent(0.f),
    m_Bitangent(0.f)
{
}

Vertex::Vertex(const glm::vec3 &position, const glm::vec3 &normal, const glm::vec2 &texCoords, const glm::vec3 &tangent,
               const glm::vec3 &biTangent): m_Position(position),
                                            m_Normal(normal),
                                            m_TexCoords(texCoords),
                                            m_Tangent(tangent),
                                            m_Bitangent(biTangent)
{
}

vk::VertexInputBindingDescription Vertex::getBindingDescription()
{
    return {0, sizeof(Vertex), vk::VertexInputRate::eVertex};
}

std::array<vk::VertexInputAttributeDescription, 5> Vertex::getAttributeDescription()
{
    return {
        vk::VertexInputAttributeDescription(0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, m_Position)),
        vk::VertexInputAttributeDescription(1, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, m_Normal)),
        vk::VertexInputAttributeDescription(2, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, m_TexCoords)),
        vk::VertexInputAttributeDescription(3, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, m_Tangent)),
        vk::VertexInputAttributeDescription(4, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, m_Bitangent))
    };
}

bool Vertex::operator==(const Vertex &other) const
{
    return m_Position == other.m_Position && m_TexCoords == other.m_TexCoords;
}
