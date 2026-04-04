#version 330 core

layout (location = 0) out vec4 outColor;
layout (location = 1) out uint outObjectID;

void main()
{
    outColor = vec4(1.0, 0.45, 0.05, 1.0);
    outObjectID = 0u;
}
