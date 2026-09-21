#version 330

in vec2 fragTexCoord;
in vec4 fragColor;
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform vec2 direction;
out vec4 finalColor;

void main() {
    vec4 sum = texture(texture0, fragTexCoord) * 0.2270270270;
    sum += texture(texture0, fragTexCoord + direction * 1.3846153846) * 0.3162162162;
    sum += texture(texture0, fragTexCoord - direction * 1.3846153846) * 0.3162162162;
    sum += texture(texture0, fragTexCoord + direction * 3.2307692308) * 0.0702702703;
    sum += texture(texture0, fragTexCoord - direction * 3.2307692308) * 0.0702702703;
    finalColor = sum * colDiffuse * fragColor;
}
