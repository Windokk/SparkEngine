#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Normals (not necessarily normalized)
layout (location = 1) in vec3 aNormal;
// Colors
layout (location = 2) in vec3 aColor;
// Texture Coordinates
layout (location = 3) in vec2 aTex;


out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

// Imports the camera matrix
uniform mat4 camMatrix;
// Imports the transformation matrices
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;


void main()
{
	// calculates current position
	FragPos = vec3(model * translation * rotation * scale * vec4(aPos, 1.0f));
	// Assigns the normal from the Vertex Data to "Normal"
	Normal = aNormal;
	// Assigns the texture coordinates from the Vertex Data to "texCoord"
	TexCoords = mat2(0.0, -1.0, 1.0, 0.0) * aTex;
	
	// Outputs the positions/coordinates of all vertices
	gl_Position = camMatrix * vec4(FragPos, 1.0);
}