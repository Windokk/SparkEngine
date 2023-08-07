#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float outlineSize = 1.6;
uniform int excludedObjectID = -1;

void main()
{
    vec4 centerColor = texture(screenTexture, TexCoords);
    
    // Check if the current pixel belongs to an excluded object
    if (excludedObjectID >= 0) {
        // Adjust this condition based on how you identify excluded objects
        if (centerColor.r == float(excludedObjectID)) {
            FragColor = centerColor; // Skip outline for excluded object pixels
            return;
        }
    }


    // Calculate the outline threshold and smoothness
    float threshold = 0.8;  // Adjust this threshold based on your scene
    float smoothness = 0.1; // Adjust this smoothness for a softer/harder edge

    // Calculate the outline factor
    float totalDifference = 0.0;
    for (float x = -outlineSize; x <= outlineSize; x += 1.0) {
        for (float y = -outlineSize; y <= outlineSize; y += 1.0) {
            vec2 offset = vec2(x, y) / textureSize(screenTexture, 0);
            vec4 sampleColor = texture(screenTexture, TexCoords + offset);
            totalDifference += length(centerColor.rgb - sampleColor.rgb);
        }
    }
    float outlineFactor = smoothstep(threshold, threshold + smoothness, totalDifference);

    // Mix the center color and outline color
    vec3 outlineColor = vec3(0.949, 0.737, 0.051); // Red outline
    FragColor = mix(centerColor, vec4(outlineColor, 1.0), outlineFactor);
    
    
}