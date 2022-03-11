/**
 * File: shader_code.cpp
 * 
 * Author: Ryan Carroll
 * 
 * Holds shader code
 */

const char VertexShaderCode[] = R"glsl(
#version 450 core

in layout(location=0) vec3 position;
in layout(location=1) vec3 color;
in layout(location=2) vec3 normal;
in layout(location=3) float diffuseStrength;
in layout(location=4) float specularStrength;

uniform mat4 vertexMatrix;
uniform mat4 normalMatrix;

out vec4 vertexPosition;
out vec3 vertexColor;
out vec3 vertexNormal;
out float vertexDiffuse;
out float vertexSpecular;

void main()
{
    vec4 tempNormal = normalMatrix * vec4(normal, 1.0);
    vec3 transformedNormal = vec3(tempNormal.x, tempNormal.y, tempNormal.z);
    vertexNormal = normalize(transformedNormal);

    vec4 v = vec4(position, 1.0f);
    vertexPosition = vertexMatrix * v;
    gl_Position = vertexPosition;
    vertexColor = color;
    vertexDiffuse = diffuseStrength;
    vertexSpecular = specularStrength;
}
)glsl";

const char FragmentShaderCode[] = R"glsl(
#version 450 core

in vec4 vertexPosition;
in vec3 vertexColor;
in vec3 vertexNormal;
in float vertexDiffuse;
in float vertexSpecular;


uniform vec3 infiniteLightDirection;
uniform vec3 infiniteLightColor;

uniform vec4 ambientLight;

uniform vec3 pointLightPosition;
uniform vec3 pointLightColor;

uniform vec3 spotLightPosition;
uniform vec3 spotLightColor;
uniform vec3 coneDirection;
uniform float spotCosCutoff;


out vec4 fragColor;

void main()
{
    vec4 color = vec4(vertexColor, 1.0);

    vec3 eyeDirection = normalize(vertexPosition.xyz);

    vec3 pointLightDirection = normalize(pointLightPosition - vec3(vertexPosition));
    vec3 spotLightDirection = normalize(spotLightPosition - vec3(vertexPosition));

    float spotCos = dot(spotLightDirection, -coneDirection);
    float spotAttenuation = 1.0;

    if (spotCos < spotCosCutoff)
        spotAttenuation = 0.0;

    vec3 infiniteHalfVector = normalize(eyeDirection + infiniteLightDirection);
    vec3 pointHalfVector = normalize(eyeDirection + pointLightDirection); // used to be diffuselight instead of lightDirection
    vec3 spotHalfVector = normalize(eyeDirection + spotLightDirection);

    float infiniteDiffuse = max(0.0, dot(vertexNormal, infiniteLightDirection));
    float pointDiffuse = max(0.0, dot(vertexNormal, pointLightDirection));
    float spotDiffuse = max(0.0, dot(vertexNormal, spotLightDirection));

    float infiniteSpecular = max(0.0, dot(vertexNormal, infiniteHalfVector));
    float pointSpecular = max(0.0, dot(vertexNormal, pointHalfVector));
    float spotSpecular = max(0.0, dot(vertexNormal, spotHalfVector));

    vec4 coloredInfiniteDiffuse = vec4(infiniteLightColor * infiniteDiffuse, 1.0);
    vec4 coloredPointDiffuse = vec4(pointLightColor * pointDiffuse, 1.0);
    vec4 coloredSpotDiffuse = vec4(spotLightColor * spotDiffuse, 1.0);

    // Just checking here if there is any light, if no light, don't specular
    if (coloredInfiniteDiffuse == vec4(vec3(0.0), 1.0))
        infiniteSpecular = 0.0;
    if (coloredPointDiffuse == vec4(vec3(0.0), 1.0))
        pointSpecular = 0.0;
    if (coloredSpotDiffuse == vec4(vec3(0.0), 1.0))
        spotSpecular = 0.0;
    
    vec4 totalDiffuse = coloredInfiniteDiffuse + coloredPointDiffuse + (coloredSpotDiffuse * spotAttenuation);
    vec4 scatteredLight = totalDiffuse + ambientLight;
    scatteredLight *= vertexDiffuse;
    // Two options here, I can use the diffuse multiplier before ambient light or after. I think after looks better

    vec4 reflectedLight = vec4(infiniteLightColor * infiniteSpecular, 1.0) + vec4(pointLightColor * pointSpecular, 1.0)
     + vec4(spotLightColor * (spotSpecular * spotAttenuation), 1.0);
    reflectedLight *= vertexSpecular;

    fragColor = min(reflectedLight + (color * scatteredLight), vec4(1.0));
}
)glsl";