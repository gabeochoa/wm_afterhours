#version 330

in vec2 fragTexCoord;
in vec4 fragColor;
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform vec2 origin;
uniform vec2 extent;
uniform float progress;
uniform float span;
out vec4 finalColor;

float hash(vec2 p) { return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453); }

float noise(vec2 p) {
    vec2 i = floor(p), f = fract(p);
    f = f * f * (3.0 - 2.0 * f);
    return mix(mix(hash(i), hash(i + vec2(1.0, 0.0)), f.x),
               mix(hash(i + vec2(0.0, 1.0)), hash(i + vec2(1.0, 1.0)), f.x), f.y);
}

void main() {
    vec2 ext = max(extent, vec2(1.0));
    vec2 uv = (gl_FragCoord.xy - origin) / ext;
    vec3 rose = vec3(0.96, 0.62, 0.74), sky = vec3(0.50, 0.66, 0.98), mint = vec3(0.60, 0.92, 0.72);
    float swirl = noise(uv * 2.5 + vec2(progress * 0.8, -progress * 0.5));
    vec3 color = mix(mix(rose, sky, clamp(uv.x + (swirl - 0.5) * 0.4, 0.0, 1.0)), mint, clamp(uv.y * 0.8 + (swirl - 0.5) * 0.3, 0.0, 1.0));
    float radius = ext.x * 0.14;
    vec2 half_extent = ext * 0.5;
    vec2 q = abs(gl_FragCoord.xy - origin - half_extent) - half_extent + vec2(radius);
    float dist = length(max(q, 0.0)) + min(max(q.x, q.y), 0.0) - radius;
    float inside = 1.0 - smoothstep(-1.0, 1.0, dist);
    float glow = exp(-max(dist, 0.0) / (ext.x * 0.12)) * 0.45 * (1.0 - smoothstep(0.0, ext.x * 0.19, dist));
    float diag = (uv.x + uv.y) * 0.5;
    float wobble = (noise(uv * 4.0 + progress * 3.0) - 0.5) * 0.3;
    float center = mix(-0.4, 1.4, progress);
    float band = 1.0 - smoothstep(0.0, span, abs(diag + wobble - center));
    color = mix(color, vec3(1.0), band * 0.55);
    finalColor = vec4(color, max(inside, glow * (0.6 + band * 0.4)) * fragColor.a);
}
