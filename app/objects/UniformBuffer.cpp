#include "UniformBuffer.h"

void UniformBuffer::cleanUp()
{
    m_Buffers.clear();

    m_Memories.clear();

    m_MappedMemories.clear();
}
