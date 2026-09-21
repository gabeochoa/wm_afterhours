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
    float n = noise(uv * 6.0) * 0.6 + noise(uv * 14.0) * 0.4;
    float v = n - uv.y * 0.25;
    float t = progress * 1.3 - 0.15;
    float alpha = 1.0 - smoothstep(t - 0.15, t + 0.05, v);
    vec3 smoke = mix(texel.rgb, vec3(0.55, 0.55, 0.6), progress);
    finalColor = vec4(smoke, texel.a * alpha);
}
