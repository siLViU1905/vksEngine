#include "TextureType.h"

namespace vks_engine
{
    aiTextureType vks_engine_TextureType_to_aiTextureType(TextureType type)
    {
        return static_cast<aiTextureType>(type);
    }
}