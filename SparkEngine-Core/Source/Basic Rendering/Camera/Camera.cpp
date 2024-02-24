#include"Camera.h"


Camera::Camera(int width, int height, glm::vec3 position, glm::vec3 rotation)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
	Orientation = rotation;
}

void Camera::updateSize(int new_width, int new_height)
{
	Camera::width = new_width;
	Camera::height = new_height;

}

void Camera::updateMatrix(float _FOVdeg, float nearPlane, float farPlane)
{
	FOVdeg = _FOVdeg;
	// Initializes matrices since otherwise they will be the null matrix
	view = glm::mat4(1.0f);
	projection = glm::mat4(1.0f);

	// Makes camera look in the right direction from the right position
	view = glm::lookAt(Position, Position + Orientation, Up);
	// Adds perspective to the scene
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);
	// Sets new camera matrix
	cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	// Exports camera matrix
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}


void Camera::Inputs(GLFWwindow* window, float speed, float sensitivity, ImVec2 viewportCenter, ImVec2 viewportSize)
{

	ImGuiIO& io = ImGui::GetIO();
	// Handles key inputs
	if (io.KeysDown[GLFW_KEY_Z])
	{
		Position += speed * Orientation;
	}
	if (io.KeysDown[GLFW_KEY_Q])
	{
		Position += speed * -glm::normalize(glm::cross(Orientation, Up));
	}
	if (io.KeysDown[GLFW_KEY_S])
	{
		Position += speed * -Orientation;
	}
	if (io.KeysDown[GLFW_KEY_D])
	{
		Position += speed * glm::normalize(glm::cross(Orientation, Up));
	}
	if (io.KeysDown[GLFW_KEY_SPACE])
	{
		Position += speed * Up;
	}
	if (io.KeysDown[GLFW_KEY_LEFT_CONTROL])
	{
		Position += speed * -Up;
	}

	// Handles mouse inputs
	if (io.MouseDown[GLFW_MOUSE_BUTTON_LEFT])
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

		if (firstClick)
		{
			firstClick = false;
			glfwGetCursorPos(window, &originalmouseX, &originalmouseY);
		}
		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);
		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = sensitivity * (float)(mouseY - round(originalmouseY)) / viewportSize.y;
		float rotY = sensitivity * (float)(mouseX - round(originalmouseX)) / viewportSize.x;

		// Calculates upcoming vertical change in the Orientation
		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

		// Decides whether or not the next vertical Orientation is legal or not
		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			Orientation = newOrientation;
		}

		// Rotates the Orientation left and right
		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);
		
		glfwSetCursorPos(window, originalmouseX, originalmouseY);
	}
	if (!io.MouseDown[GLFW_MOUSE_BUTTON_LEFT] && !firstClick)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange, ImGuiConfigFlags_DockingEnable;
		// Makes sure the next time the camera looks around it doesn't jump
		firstClick = true;
	}
}