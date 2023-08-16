#version 330 core

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D screenTexture;

void main()
{
    vec4 fragment = texture(screenTexture, texCoords);
    float gamma = 0.8;
    FragColor.rgb = pow(fragment.rgb, vec3(1.0/gamma));
}