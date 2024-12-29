#version 330 core
layout(location = 0) in vec3 position;

uniform mat4 MVP;

out vec2 UV;

void main() {
    gl_Position = MVP * vec4(position, 1.0);
    UV = position.xz * 0.1; // Scale UV for tiling
}
