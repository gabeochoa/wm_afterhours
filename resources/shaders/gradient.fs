#version 330

in vec2 fragTexCoord;
in vec4 fragColor;
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform float time;
uniform vec2 origin;
uniform vec2 extent;
uniform float radius;
uniform float softness;
out vec4 finalColor;

vec3 hue(float h) {
    vec3 k = mod(vec3(5.0, 3.0, 1.0) + h * 6.0, 6.0);
    return 1.0 - clamp(min(k, 4.0 - k), 0.0, 1.0);
}

void main() {
    vec4 texel = texture(texture0, fragTexCoord) * colDiffuse * fragColor;
    vec2 uv = (gl_FragCoord.xy - origin) / max(extent, vec2(1.0));
    float wash = 0.5 + 0.5 * sin(uv.x * 4.0 + time * 1.2566) * cos(uv.y * 3.0 - time * 1.0053);
    vec3 color = mix(hue(fract(time * 0.25 + uv.x * 0.3)), hue(fract(time * 0.25 + 0.5 + uv.y * 0.3)), wash);
    vec2 half_extent = max(extent, vec2(1.0)) * 0.5;
    vec2 q = abs(gl_FragCoord.xy - origin - half_extent) - half_extent + vec2(radius);
    float dist = length(max(q, 0.0)) + min(max(q.x, q.y), 0.0) - radius;
    float edge = softness > 0.0 ? 1.0 - smoothstep(-softness, 0.0, dist) : 1.0;
    finalColor = vec4(mix(color, color * 1.15, wash), texel.a * edge);
}
