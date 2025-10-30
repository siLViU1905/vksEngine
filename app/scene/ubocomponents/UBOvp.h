#ifndef VKSENGINE_UBOMVP_H
#define VKSENGINE_UBOMVP_H
#include <glm/glm.hpp>

namespace vks_engine
{
    struct UBOvp
    {
        glm::mat4 view;
        glm::mat4 proj;
        glm::vec3 viewPos;
        float padding;
    };
}
#endif //VKSENGINE_UBOMVP_H
