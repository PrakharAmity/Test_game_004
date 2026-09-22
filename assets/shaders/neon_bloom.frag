// Neon Bloom & Chromatic Aberration Post-Processing Shader
// PixelVerse Legends — Cyber Fantasy Rendering Pipeline
#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D u_texture;
uniform vec2 u_resolution;
uniform float u_time;
uniform float u_intensity;

void main() {
    vec2 uv = gl_FragCoord.xy / u_resolution.xy;
    
    // Chromatic Aberration Offset
    float dist = distance(uv, vec2(0.5, 0.5));
    vec2 offset = (uv - vec2(0.5, 0.5)) * dist * 0.015;
    
    float r = texture2D(u_texture, uv + offset).r;
    float g = texture2D(u_texture, uv).g;
    float b = texture2D(u_texture, uv - offset).b;
    
    // Sample bloom blur
    vec4 sceneColor = vec4(r, g, b, 1.0);
    vec4 bloomColor = vec4(0.0);
    
    float blurRadius = 0.005;
    for (float x = -2.0; x <= 2.0; x += 1.0) {
        for (float y = -2.0; y <= 2.0; y += 1.0) {
            vec2 sampleUv = uv + vec2(x, y) * blurRadius;
            vec4 col = texture2D(u_texture, sampleUv);
            float brightness = dot(col.rgb, vec3(0.2126, 0.7152, 0.0722));
            if (brightness > 0.6) {
                bloomColor += col * (brightness - 0.6) * 1.5;
            }
        }
    }
    bloomColor /= 25.0;
    
    // Add pulsing neon energy tint (Cyan & Electric Violet)
    vec3 neonTint = vec3(0.0, 0.94, 1.0) * (0.05 + 0.03 * sin(u_time * 3.0));
    
    gl_FragColor = sceneColor + bloomColor * u_intensity + vec4(neonTint, 0.0);
}
