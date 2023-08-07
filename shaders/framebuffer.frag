#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;

mat3 sx = mat3( 
    1.0, 2.0, 1.0, 
    0.0, 0.0, 0.0, 
   -1.0, -2.0, -1.0 
);

mat3 sy = mat3( 
    1.0, 0.0, -1.0, 
    2.0, 0.0, -2.0, 
    1.0, 0.0, -1.0 
);

void main()
{
    
    // Calculate the luminance (brightness) of the pixel
    vec3 diffuse = texture(screenTexture, TexCoords).rgb;
    mat3 I;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            vec3 samp = texelFetch(screenTexture, ivec2(TexCoords.xy * vec2(textureSize(screenTexture, 0))) + ivec2(i - 1, j - 1), 0).rgb;
            I[i][j] = length(samp); 
        }
    }
    float gx = dot(sx[0], I[0]) + dot(sx[1], I[1]) + dot(sx[2], I[2]); 
    float gy = dot(sy[0], I[0]) + dot(sy[1], I[1]) + dot(sy[2], I[2]);

    float g = sqrt(pow(gx, 8.0) + pow(gy, 8.0));
    FragColor = vec4(diffuse +vec3(g), 1.0);
    
    
}