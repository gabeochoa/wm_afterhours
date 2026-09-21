#version 330

in vec2 fragTexCoord;
in vec4 fragColor;
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform float amount;
out vec4 finalColor;

void main() {
    vec2 uv = fragTexCoord;
    uv.x += amount * 0.06 * sin(uv.y * 3.14159);
    uv.y += amount * 0.04 * sin(uv.x * 6.28318 + 1.0);
    float inside = step(0.0, uv.x) * step(uv.x, 1.0) * step(0.0, uv.y) * step(uv.y, 1.0);
    vec4 texel = texture(texture0, clamp(uv, 0.0, 1.0)) * colDiffuse * fragColor;
    finalColor = vec4(texel.rgb, texel.a * inside);
}
