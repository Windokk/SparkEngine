#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include<string>

#include"../VAO/VAO.h"
#include"../EBO/EBO.h"
#include"../Camera/Camera.h"
#include"../Texture/Texture.h"
#include"../../Utils/Engine/EngineUtils.h"

class Mesh
{
public:
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;
	// Store VAO in public so it can be used in the Draw function
	VAO VAO;
	
	unsigned int instancing;

	// Initializes the mesh
	Mesh(std::vector <Vertex>& vertices,
		std::vector <GLuint>& indices,
		std::vector <Texture>& textures,
		unsigned int instancing = 1,
		std::vector <glm::mat4> instanceMatrix = {});

	// Draws the mesh
	void DrawPathTraced(Shader& shader, Camera& camera, glm::mat4 matrix, glm::vec3 translation, glm::quat rotation, glm::vec3 scale, std::vector<light_Infos> lights, PathTracingInfos infos);
	void Draw(Shader& shader, Camera& camera, glm::mat4 matrix, glm::vec3 translation, glm::quat rotation, glm::vec3 scale, std::vector<light_Infos> lights);
	
};
#endif