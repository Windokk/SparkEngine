#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>


#include "../../Utils/Engine/EngineUtils.h"
#include"../Shader/Shader.h"
#include "../../Libs/ImGui_Lib/imgui.h"

class Camera
{
public:
	// Stores the main vectors of the camera
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 cameraMatrix = glm::mat4(1.0f);
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Orientation, Up);
	}
	// Prevents the camera from jumping around when first clicking left click
	bool firstClick = true;
	double originalmouseX = 0.0, originalmouseY = 0.0;
	// Stores the width and height of the window
	int width;
	int height;

	// Camera constructor to set up initial values
	Camera(int width, int height, glm::vec3 position, glm::vec3 rotation);

	void updateSize(int new_width, int new_height);

	// Updates the camera matrix to the Vertex Shader
	void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
	// Exports the camera matrix to a shader
	void Matrix(Shader& shader, const char* uniform);
	// Handles camera inputs
	void Inputs(GLFWwindow* window, float speed = 0.01f, float sensitivity = 100.0f, ImVec2 viewportCenter = ImVec2(0, 0), ImVec2 viewportSize = ImVec2(0, 0));
};
#endif