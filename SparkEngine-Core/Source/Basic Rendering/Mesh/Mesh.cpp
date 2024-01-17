#include "Mesh.h"

Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures, unsigned int instancing, std::vector <glm::mat4> instanceMatrix)
{
	Mesh::vertices = vertices;
	Mesh::indices = indices;
	Mesh::textures = textures;
	Mesh::instancing = instancing;

	VAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO instanceVBO(instanceMatrix);
	VBO VBO(vertices);
	// Generates Element Buffer Object and links it to indices
	EBO EBO(indices);
	// Links VBO attributes such as coordinates and colors to VAO
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));
	if (instancing != 1)
	{
		instanceVBO.Bind();
		// Can't link to a mat4 so you need to link four vec4s
		VAO.LinkAttrib(instanceVBO, 4, 4, GL_FLOAT, sizeof(glm::mat4), (void*)0);
		VAO.LinkAttrib(instanceVBO, 5, 4, GL_FLOAT, sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)));
		VAO.LinkAttrib(instanceVBO, 6, 4, GL_FLOAT, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		VAO.LinkAttrib(instanceVBO, 7, 4, GL_FLOAT, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
		// Makes it so the transform is only switched when drawing the next instance
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);
	}
	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	instanceVBO.Unbind();
	VBO.Unbind();
	EBO.Unbind();


}

void Mesh::Draw(Shader& shader, Camera& camera, glm::mat4 matrix, glm::vec3 translation, glm::quat rotation, glm::vec3 scale, std::vector<light_Infos> lights)
{
	// Bind shader to be able to access uniforms

	shader.Activate();
	VAO.Bind();

	// Keep track of how many of each type of textures we have
	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		std::string num;
		std::string type = textures[i].type;
		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular")
		{
			num = std::to_string(numSpecular++);
		}
		textures[i].texUnit(shader, (type + num).c_str(), i);
		textures[i].Bind();
	}

	// Take care of the camera Matrix
	shader.setVec3("camPos", camera.Position);
	camera.Matrix(shader, "camMatrix");
	shader.setFloat("material.shininess", 32.0f);
	int numDirLights = 0;
	int numPointLights = 0;
	int numSpotLights = 0;
	for (int i = 0; i < lights.size(); i++) {
		switch (lights[i].type) {
			case LT_DIRECTIONNAL:
				numDirLights += 1;
				shader.setVec3("dirLights[" + std::to_string(numDirLights - 1) + "].direction", lights[i].direction);
				shader.setVec3("dirLights[" + std::to_string(numDirLights - 1) + "].ambient", lights[i].ambient);
				shader.setVec3("dirLights[" + std::to_string(numDirLights - 1) + "].diffuse", lights[i].diffuse);
				shader.setVec3("dirLights[" + std::to_string(numDirLights - 1) + "].specular", lights[i].specular);
				shader.setFloat("dirLights[" + std::to_string(numDirLights - 1) + "].intensity", lights[i].intensity);
				shader.setVec3("dirLights[" + std::to_string(numDirLights - 1) + "].color", lights[i].color);

				break;
			case LT_POINT:
				numPointLights += 1;
				shader.setVec3("pointLights[" + std::to_string(numPointLights - 1) + "].position", lights[i].position);
				shader.setVec3("pointLights[" + std::to_string(numPointLights - 1) + "].ambient", lights[i].ambient);
				shader.setVec3("pointLights[" + std::to_string(numPointLights - 1) + "].diffuse", lights[i].diffuse);
				shader.setVec3("pointLights[" + std::to_string(numPointLights - 1) + "].specular", lights[i].specular);
				shader.setFloat("pointLights[" + std::to_string(numPointLights - 1) + "].constant", lights[i].constant);
				shader.setFloat("pointLights[" + std::to_string(numPointLights - 1) + "].linear", lights[i].linear);
				shader.setFloat("pointLights[" + std::to_string(numPointLights - 1) + "].quadratic", lights[i].quadratic);
				shader.setFloat("pointLights[" + std::to_string(numPointLights - 1) + "].intensity", lights[i].intensity);
				shader.setVec3("pointLights[" + std::to_string(numPointLights - 1) + "].color", lights[i].color);
				break;
			case LT_SPOT:
				numSpotLights += 1;
				shader.setVec3("spotLights[" + std::to_string(numSpotLights - 1) + "].position", lights[i].position);
				shader.setVec3("spotLights[" + std::to_string(numSpotLights - 1) + "].direction", lights[i].direction);
				shader.setVec3("spotLights[" + std::to_string(numSpotLights - 1) + "].ambient", lights[i].ambient);
				shader.setVec3("spotLights[" + std::to_string(numSpotLights - 1) + "].diffuse", lights[i].diffuse);
				shader.setVec3("spotLights[" + std::to_string(numSpotLights - 1) + "].specular", lights[i].specular);
				shader.setFloat("spotLights[" + std::to_string(numSpotLights - 1) + "].constant", lights[i].constant);
				shader.setFloat("spotLights[" + std::to_string(numSpotLights - 1) + "].linear", lights[i].linear);
				shader.setFloat("spotLights[" + std::to_string(numSpotLights - 1) + "].quadratic", lights[i].quadratic);
				shader.setFloat("spotLights[" + std::to_string(numSpotLights - 1) + "].cutOff", lights[i].cutOff);
				shader.setFloat("spotLights[" + std::to_string(numSpotLights - 1) + "].outerCutOff", lights[i].outerCutOff);
				shader.setFloat("spotLights[" + std::to_string(numSpotLights - 1) + "].intensity", lights[i].intensity);
				shader.setVec3("spotLights[" + std::to_string(numSpotLights - 1) + "].color", lights[i].color);
				break;
		}
	}

	shader.setInt("numDirLights", numDirLights);
	shader.setInt("numPointLights", numPointLights);
	shader.setInt("numSpotLights", numSpotLights);

	if (instancing == 1)
	{
		// Initialize matrices
		glm::mat4 trans = glm::mat4(1.0f);
		glm::mat4 rot = glm::mat4(1.0f);
		glm::mat4 sca = glm::mat4(1.0f);

		// Transform the matrices to their correct form
		trans = glm::translate(trans, translation);
		rot = glm::mat4_cast(rotation);
		sca = glm::scale(sca, scale);

		// Push the matrices to the vertex shader
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(trans));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(sca));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(matrix));

		// Draw the actual mesh
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, instancing);
	}
}
