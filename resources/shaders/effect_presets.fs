#version 330

in vec2 fragTexCoord;
in vec4 fragColor;
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform vec2 origin;
uniform vec2 extent;
uniform float progress;
uniform float mode;
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
    int m = int(mode + 0.5);
    float alpha = 1.0;
    vec3 rgb = texel.rgb;
    if (m == 0) {
        float n = noise(uv * 9.0) * 0.6 + hash(floor(gl_FragCoord.xy)) * 0.4;
        alpha = smoothstep(progress * 1.4 - 0.2, progress * 1.4 + 0.2, n + uv.y * 0.3);
        rgb = mix(rgb, vec3(0.65, 0.65, 0.7), progress * 0.4);
    } else if (m == 1) {
        alpha = step(uv.x, progress);
    } else if (m == 2) {
        alpha = step(abs(uv.x - 0.5), progress * 0.5);
    } else if (m == 3) {
        alpha = step((uv.x + uv.y) * 0.5, progress);
    } else if (m == 4) {
        alpha = step(distance(uv, vec2(0.5)), progress * 0.75);
    } else if (m == 5) {
        float d = distance(uv, vec2(0.5));
        rgb *= mix(0.25, 1.0, 1.0 - smoothstep(progress * 0.45, progress * 0.45 + 0.15, d));
    } else if (m == 6) {
        float band = floor(uv.y * 8.0);
        alpha = step(fract(uv.y * 8.0), progress) ;
        rgb *= 0.85 + 0.15 * hash(vec2(band, 1.0));
    } else if (m == 7) {
        float front = 1.0 - progress;
        alpha = step(front, uv.y);
        rgb *= 1.0 - 0.35 * (1.0 - smoothstep(0.0, 0.12, abs(uv.y - front)));
    }
    finalColor = vec4(rgb, texel.a * alpha);
}
