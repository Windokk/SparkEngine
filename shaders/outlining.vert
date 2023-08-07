#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;


vec3 Normal;
vec3 crntPos_transform;

uniform float size;
uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;

void main()
{
	
	// Third Method
	Normal = aNormal;
	crntPos_transform = vec3(model * translation * rotation * scale * vec4(aPos, 1.0f));
	gl_Position = camMatrix * vec4(crntPos_transform*size, 1.0);
}