#version 460

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

layout(location = 0) out vec3 fragPos;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec2 fragTexCoords;
layout(location = 3) out vec3 viewPos;
layout(location = 4) out mat3 TBN;

layout(binding = 0) uniform MVP
{
    mat4 view;
    mat4 projection;
    vec3 viewpos;
}
vp;

layout(push_constant) uniform PushConstants {
    mat4 model;
} pushConst;

void main()
{
    mat3 normalMatrix = mat3(transpose(inverse(pushConst.model)));

    fragPos = vec3(pushConst.model * vec4(aPos, 1.0));

    fragTexCoords = aTexCoords;

    viewPos = vp.viewpos;

    vec3 T = normalize(vec3(pushConst.model * vec4(aTangent, 0.0)));

    vec3 B = normalize(vec3(pushConst.model * vec4(aBitangent, 0.0)));

    vec3 N = normalize(normalMatrix * aNormal);

    TBN = mat3(T, B, N);

    fragNormal = N;

    gl_Position = vp.projection * vp.view * vec4(fragPos, 1.0);
}