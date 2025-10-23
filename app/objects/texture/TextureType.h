#ifndef VKSENGINETEXTURETYPE_H
#define VKSENGINETEXTURETYPE_H
#include "assimp/material.h"
#include <unordered_map>

namespace vks_engine
{
    enum class TextureType
    {
        NONE = aiTextureType_NONE,
        DIFFUSE = aiTextureType_DIFFUSE,
        SPECULAR = aiTextureType_SPECULAR,
        NORMALS = aiTextureType_NORMALS
    };

    aiTextureType vks_engine_TextureType_to_aiTextureType(TextureType type);
}

template<>
struct std::hash<vks_engine::TextureType>
{
    size_t operator()(const vks_engine::TextureType &type) const noexcept
    {
        return std::hash<int>()(static_cast<int>(type));
    }
};

#endif //VKSENGINETEXTURETYPE_H
