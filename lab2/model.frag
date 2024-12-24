#version 330 core

out vec4 FragColor;

in vec3 FragPos;   // Position from vertex shader
in vec3 Normal;    // Normal vector
in vec2 TexCoords; // Texture coordinates

uniform sampler2D texture1; // Model's texture

void main()
{
    vec3 color = texture(texture1, TexCoords).rgb; // Get texture color
    FragColor = vec4(color, 1.0);
}
