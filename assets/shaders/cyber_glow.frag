#version 330 core
// PixelVerse Legends — Cyber Glow Fragment Shader
in vec2 TexCoords;
in vec4 FragColor;
out vec4 FinalColor;

uniform sampler2D screenTexture;
uniform float u_time;
uniform vec3 u_glowColor;
uniform float u_intensity;

void main() {
    vec4 baseColor = texture(screenTexture, TexCoords) * FragColor;
    
    // Calculate chromatic aberration pulse
    vec2 offset = vec2(sin(u_time * 2.0) * 0.002, cos(u_time * 2.0) * 0.002);
    float r = texture(screenTexture, TexCoords + offset).r;
    float g = texture(screenTexture, TexCoords).g;
    float b = texture(screenTexture, TexCoords - offset).b;
    
    vec3 chromatic = vec3(r, g, b);
    vec3 neonGlow = u_glowColor * u_intensity * (0.8 + 0.2 * sin(u_time * 4.0));
    
    FinalColor = vec4(mix(baseColor.rgb, chromatic + neonGlow, 0.3), baseColor.a);
}
