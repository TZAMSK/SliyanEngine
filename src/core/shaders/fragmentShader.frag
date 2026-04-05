#version 330 core

in vec3 vColor;

layout (location = 0) out vec4 outColor;
layout (location = 1) out uint outObjectID;

uniform uint objectID;
uniform int isHovered;
uniform int isSelected;

void main()
{
    vec3 color = vColor;

    if (isSelected == 1 || isHovered == 1)
        color = mix(color, vec3(1.0, 0.72, 0.25), 0.05);
    
    outColor = vec4(color, 1.0);
    outObjectID = objectID;
}
