#ifndef UBOMVP_H
#define UBOMVP_H
#include <glm/glm.hpp>

template<uint32_t modelCount>
struct UBOmvp
{
    glm::mat4 model[modelCount];
    glm::mat4 view;
    glm::mat4 proj;
    glm::vec3 viewPos;
    float padding;
};

#endif //UBOMVP_H
