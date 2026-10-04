#version 330 core

in vec3 vColor;
in vec3 vWorldPos;
in vec4 vLightSpacePos;

layout (location = 0) out vec4 outColor;
layout (location = 1) out uint outObjectID;

uniform uint objectID;
uniform int  isHovered;
uniform int  isSelected;
uniform int  useLighting = 1;

uniform sampler2D shadowMap;

uniform vec3 lightDir = vec3(0.5, 0.3, 1.0);

const float AMBIENT = 0.35;

float calculateShadow(vec4 lightSpacePos, float bias)
{
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0 || projCoords.z < 0.0 ||
        any(lessThan(projCoords.xy, vec2(0.0))) ||
        any(greaterThan(projCoords.xy, vec2(1.0))))
        return 0.0;

    float currentDepth = projCoords.z;

    vec2 texelSize = 1.0 / vec2(textureSize(shadowMap, 0));
    float shadow = 0.0;

    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float closestDepth =
                texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += (currentDepth - bias > closestDepth) ? 1.0 : 0.0;
        }
    }

    return shadow / 9.0;
}

void main()
{
    vec3 color = vColor;

    if (isSelected == 1 || isHovered == 1)
        color = mix(color, vec3(1.0, 0.72, 0.25), 0.05);

    if (useLighting == 0)
    {
        outColor = vec4(color, 1.0);
        outObjectID = objectID;
        return;
    }

    vec3 normal = normalize(cross(dFdx(vWorldPos), dFdy(vWorldPos)));
    vec3 L = normalize(lightDir);

    float diff = max(dot(normal, L), 0.0);
    float bias = max(0.01 * (1.0 - diff), 0.002);

    float shadow = calculateShadow(vLightSpacePos, bias);

    float lighting = AMBIENT + (1.0 - AMBIENT) * diff * (1.0 - shadow);

    outColor = vec4(color * lighting, 1.0);
    outObjectID = objectID;
}
