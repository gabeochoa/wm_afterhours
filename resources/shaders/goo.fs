#version 330

in vec2 fragTexCoord;
in vec4 fragColor;
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform vec2 c0;
uniform vec2 c1;
uniform vec2 c2;
uniform vec2 c3;
uniform float r0;
uniform float r1;
uniform float r2;
uniform float r3;
out vec4 finalColor;

float ball(vec2 p, vec2 c, float r) {
    float d = max(length(p - c), 0.001);
    float q = r / d;
    return q * q * q;
}

void main() {
    vec4 texel = texture(texture0, fragTexCoord) * colDiffuse * fragColor;
    vec2 p = gl_FragCoord.xy;
    float field = ball(p, c0, r0) + ball(p, c1, r1) + ball(p, c2, r2) + ball(p, c3, r3);
    float alpha = smoothstep(0.9, 1.1, field);
    finalColor = vec4(texel.rgb, texel.a * alpha);
}
