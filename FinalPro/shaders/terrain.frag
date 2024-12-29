#version 330 core

in vec3 worldPosition;
in vec3 worldNormal;
in vec2 uv;

uniform sampler2D textureSampler;

out vec3 finalColor;

uniform vec3 lightPosition;
uniform vec3 lightIntensity;
uniform float exposure;

uniform sampler2D shadowMap;
uniform mat4 lightSpaceMatrix;

void main()
{
    vec3 normal = normalize(worldNormal);
    vec3 lightDirection = normalize(lightPosition - worldPosition);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = diff * lightIntensity;

    vec4 fragPosLightSpace = lightSpaceMatrix * vec4(worldPosition, 1.0);
    vec3 lightCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    lightCoords = lightCoords * 0.5 + 0.5;

    float shadow = 1.0;
    if (!(lightCoords.x < 0.0 || lightCoords.x > 1.0 || lightCoords.y < 0.0 || lightCoords.y > 1.0 || lightCoords.z > 1.0)) {
        float closestDepth = texture(shadowMap, lightCoords.xy).r;
        float bias = max(0.05 * (1.0 - dot(normal, lightDirection)), 0.005);
        shadow = (lightCoords.z >= closestDepth + bias) ? 0.2 : 1.0;
    }

    diffuse *= shadow;
    vec3 exposedColor = diffuse * exposure;
    vec3 toneMappedColor = exposedColor / (exposedColor + vec3(1.0));
    finalColor = texture(textureSampler, uv).rgb * pow(toneMappedColor, vec3(1.0 / 2.2));
}
