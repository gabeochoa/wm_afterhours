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
    vec3 rose = vec3(0.98, 0.80, 0.86), sky = vec3(0.78, 0.86, 0.98), mint = vec3(0.86, 0.95, 0.82);
    vec3 color = mix(mix(rose, sky, clamp(uv.x, 0.0, 1.0)), mint, clamp(uv.y, 0.0, 1.0) * 0.5);
    float radius = ext.x * 0.12;
    vec2 half_extent = ext * 0.5;
    vec2 q = abs(gl_FragCoord.xy - origin - half_extent) - half_extent + vec2(radius);
    float dist = length(max(q, 0.0)) + min(max(q.x, q.y), 0.0) - radius;
    float inside = 1.0 - smoothstep(-1.0, 1.0, dist);
    float glow = exp(-max(dist, 0.0) / (ext.x * 0.1)) * 0.55;
    float diag = (uv.x + uv.y) * 0.5;
    float wobble = (noise(uv * 3.0 + progress * 2.0) - 0.5) * 0.35;
    float center = mix(-0.5, 1.5, progress);
    float opening = 1.0 - smoothstep(0.0, span * 0.5, abs(diag + wobble - center) - span * 0.5);
    finalColor = vec4(color, max(inside, glow) * opening * fragColor.a);
}
