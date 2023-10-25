#include <iostream>
#include <windows.h>
#include <glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<stb/stb_image_write.h>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "srcs/imgui/imgui.h"
#include "srcs/imgui/imgui_impl_glfw.h"
#include "srcs/imgui/imgui_impl_opengl3.h"

#include "srcs/SceneLoader.h"
#include "srcs/EngineUtils.h"
#include "srcs/imgui/imgui_internal.h"
#include "srcs/Line.h"
#include "srcs/ImGuiMain.h"

unsigned int width_ = 1280;
unsigned int height_ = 720;

unsigned int anti_aliasing_samples = 8;

int selectedObjectID;

SceneLoader loader;

ImGuiMain gui = ImGuiMain();

const char* current_scene = "./assets/defaults/scenes/test.json";

Camera cam = Camera(0, 0, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));

void windowclosecallback(GLFWwindow* window) {
	SaveTextureToFile(loader.postProcessingTexture, width_, height_, "./assets/generated/screenshots/texture.png");
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main() {
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "Crab Engine"
	GLFWwindow* window = glfwCreateWindow(width_, height_, "Crab Engine", NULL, NULL);

	// Sets the window's icons
	GLFWimage images[2];
	images[0].pixels = stbi_load("assets/defaults/logos/icon.png", &images[0].width, &images[0].height, 0, 4);
	images[1].pixels = stbi_load("assets/defaults/logos/icon.png", &images[1].width, &images[1].height, 0, 4);
	glfwSetWindowIcon(window, 1, images);
	stbi_image_free(images[0].pixels);
	stbi_image_free(images[1].pixels);

	glfwSetWindowCloseCallback(window, windowclosecallback);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();

	//Creates a loader object to load scenes
	loader.width_ = width_;
	loader.height_ = height_;
	loader.anti_aliasing_samples = anti_aliasing_samples;

	// Creates camera object
	cam = Camera(width_, height_, glm::vec3(-2.75603, 4.46763, -2.21178), glm::vec3(0, 0, 1));

	// Variables to create periodic event for FPS displaying
	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	// Keeps track of the amount of frames in timeDiff
	unsigned int counter = 0;
	std::string FPS = "0.0";

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	gui.Load(window, io);

	loader.LoadNewScene(current_scene);

	while (!glfwWindowShouldClose(window)) {
		// Updates counter and times
		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;
		counter++;
		if (timeDiff >= 1.0 / 30.0)
		{
			// Creates new title
			std::string NEW_FPS = std::to_string((1.0 / timeDiff) * counter);
			if (stof(NEW_FPS) >= stof(FPS)) {
				FPS = NEW_FPS;
			}
			std::string ms = std::to_string((timeDiff / counter) * 1000);
			std::string newTitle = "CrabEngine - " + FPS + "FPS / " + ms + "ms";
			glfwSetWindowTitle(window, newTitle.c_str());
			// Resets times and counter
			prevTime = crntTime;
			counter = 0;
			// Use this if you have disabled VSync
			//camera.Inputs(window);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, loader.FBO);

		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		// Enable depth testing since it's disabled when drawing the framebuffer rectangle
		glEnable(GL_DEPTH_TEST);

		//Update the scene from the loader
		loader.Update(cam);

		glDepthFunc(GL_LEQUAL);

		loader.skyboxShader.Activate();
		glm::mat4 view = glm::mat4(glm::mat3(glm::lookAt(cam.Position, cam.Position + cam.Orientation, cam.Up)));
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width_ / height_, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(loader.skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(loader.skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// Draws the cubemap as the last object so we can save a bit of performance by discarding all fragments
		// where an object is present (a depth of 1.0f will always fail against any object's depth value)
		glBindVertexArray(loader.skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, loader.cubemapTexture);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		// Switch back to the normal depth function
		glDepthFunc(GL_LESS);

		view = glm::mat4(glm::mat3(glm::lookAt(cam.Position, cam.Position + cam.Orientation, cam.Up)));
		projection = glm::perspective(glm::radians(45.0f), (float)width_ / height_, 0.1f, 100.0f);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, loader.FBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, loader.postProcessingFBO);
		// Conclude the multisampling and copy it to the post-processing FBO
		glBlitFramebuffer(0, 0, width_, height_, 0, 0, width_, height_, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		// Bind the default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//We draw the ImGui interface
		gui.Draw(window, cam, loader, selectedObjectID, io);

		// Updates and exports the camera matrix to the Vertex Shader
		cam.updateMatrix(45.0f, 0.1f, 100.0f);

		glfwSwapInterval(0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	loader.Unload();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}