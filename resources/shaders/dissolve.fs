#version 330

in vec2 fragTexCoord;
in vec4 fragColor;
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform vec2 origin;
uniform vec2 extent;
uniform float progress;
out vec4 finalColor;

float hash(vec2 p) { return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453); }

float noise(vec2 p) {
    vec2 i = floor(p), f = fract(p);
    f = f * f * (3.0 - 2.0 * f);
    return mix(mix(hash(i), hash(i + vec2(1.0, 0.0)), f.x),
               mix(hash(i + vec2(0.0, 1.0)), hash(i + vec2(1.0, 1.0)), f.x), f.y);
}

void main() {
    vec4 texel = texture(texture0, fragTexCoord) * colDiffuse * fragColor;
    vec2 uv = (gl_FragCoord.xy - origin) / max(extent, vec2(1.0));
    vec2 drift = vec2(0.0, progress * 0.6);
    float n = noise(uv * 9.0 + drift) * 0.45 + noise(uv * 22.0 + drift * 1.7) * 0.35 + hash(floor(gl_FragCoord.xy)) * 0.2;
    float v = n + uv.y * 0.35;
    float t = progress * 1.55 - 0.2;
    float alpha = smoothstep(t - 0.1, t + 0.1, v);
    float edge = alpha * (1.0 - smoothstep(t + 0.1, t + 0.3, v));
    vec3 smoke = mix(texel.rgb, vec3(0.62, 0.62, 0.68), clamp(progress * 0.5, 0.0, 1.0));
    smoke = mix(smoke, vec3(0.9, 0.9, 0.95), edge * 0.35);
    finalColor = vec4(smoke, texel.a * alpha * (1.0 - 0.25 * progress));
}
