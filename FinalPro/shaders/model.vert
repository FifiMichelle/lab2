#version 330 core

layout (location = 0) in vec3 aPosition; // Vertex position
layout (location = 1) in vec3 aNormal;   // Vertex normal
layout (location = 2) in vec2 aTexCoords; // Texture coordinates

uniform mat4 uModel;       // Model matrix
uniform mat4 uView;        // View matrix (from camera)
uniform mat4 uProjection;  // Projection matrix

out vec3 FragPos;          // Position of fragment in world space
out vec3 Normal;           // Normal vector in world space
out vec2 TexCoords;        // Texture coordinates

void main()
{
    FragPos = vec3(uModel * vec4(aPosition, 1.0)); // Transform to world space
    Normal = mat3(transpose(inverse(uModel))) * aNormal; // Transform normals
    TexCoords = aTexCoords; // Pass texture coordinates to fragment shader
    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
}
