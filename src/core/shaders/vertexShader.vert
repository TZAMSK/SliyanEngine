#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vColor;
out vec3 vWorldPos;
out vec4 vLightSpacePos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 lightSpaceMatrix;

uniform vec3 shapeColor;
uniform int useVertexColor;

void main()
{
    vColor = (useVertexColor == 1) ? aColor : shapeColor;

    vec4 world = model * vec4(aPos, 1.0);
    vWorldPos = world.xyz;
    vLightSpacePos = lightSpaceMatrix * world;

    gl_Position = proj * view * world;
}
