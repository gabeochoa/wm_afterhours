#version 330

in vec2 fragTexCoord;
in vec4 fragColor;
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform vec4 tint;
out vec4 finalColor;

void main() {
    vec4 texel = texture(texture0, fragTexCoord) * colDiffuse * fragColor;
    float luma = dot(texel.rgb, vec3(0.299, 0.587, 0.114));
    finalColor = vec4(mix(texel.rgb, tint.rgb * luma, tint.a), texel.a);
}
