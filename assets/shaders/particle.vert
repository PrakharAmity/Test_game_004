#version 330 core
// PixelVerse Legends — Cyber Particle Vertex Shader
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec4 aColor;

out vec2 TexCoords;
out vec4 FragColor;

uniform mat4 projection;
uniform mat4 view;

void main() {
    TexCoords = aTexCoords;
    FragColor = aColor;
    gl_Position = projection * view * vec4(aPos, 1.0);
}
