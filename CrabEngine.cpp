#include <iostream>
#include <windows.h>
#include <glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "srcs/imgui/imgui.h"
#include "srcs/imgui/imgui_impl_glfw.h"
#include "srcs/imgui/imgui_impl_opengl3.h"

#include "srcs/SceneLoader.h"
#include "srcs/Utils.h"

unsigned int width_ = 1280;
unsigned int height_ = 720;
unsigned int samples = 8;
SceneLoader loader;
bool isLoadingScene = false;

const char* current_scene = "./assets/defaults/scenes/test.json";

Camera cam = Camera(0, 0, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));

void LoadNewScene(const char* scene) {
	isLoadingScene = true;
	loader.Load1(scene);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_MULTISAMPLE);

	glEnable(GL_FRAMEBUFFER_SRGB);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	loader.Load2();

	isLoadingScene = false;
}

bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
	// Load from file
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create a OpenGL texture identifier
	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

	// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	stbi_image_free(image_data);

	*out_texture = image_texture;
	*out_width = image_width;
	*out_height = image_height;

	return true;
}

GLuint LoadImageTexture(const char* path) {
	int my_image_width = 0;
	int my_image_height = 0;
	GLuint my_image_texture = 0;
	bool ret = LoadTextureFromFile(path, &my_image_texture, &my_image_width, &my_image_height);
	IM_ASSERT(ret);
	return my_image_texture;
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
	loader.samples = samples;

	// Enables the Depth Buffer

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
	ImGuiIO& io = ImGui::GetIO(); (void)io;;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBringToFrontOnFocus;
	io.ConfigFlags |= (ImGuiConfigFlags_NoMouseCursorChange, ImGuiConfigFlags_DockingEnable);

	bool isHoverViewport;



	LoadNewScene(current_scene);

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

		if (!isLoadingScene) {
			for (int i = 0; i < loader.models.size(); i++) {
				std::string shader_name;
				shader_name = loader.parser.models[i].shader;
				int shader_id = 0;
				for (int a = 0; a < loader.shaders.size(); a++) {
					if (loader.shaders[a].name == shader_name) {
						shader_id = a;
					}

				}

				loader.models[i].Draw(loader.shaders[shader_id].shader, cam, loader.parser.models[i].type, loader.parser.models[i].location, loader.parser.models[i].rotation, loader.parser.models[i].scale);

			}
		}


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

		glBindFramebuffer(GL_READ_FRAMEBUFFER, loader.FBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, loader.postProcessingFBO);
		// Conclude the multisampling and copy it to the post-processing FBO
		glBlitFramebuffer(0, 0, width_, height_, 0, 0, width_, height_, GL_COLOR_BUFFER_BIT, GL_NEAREST);


		// Bind the default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Draw the framebuffer rectangle
		loader.framebufferProgram.Activate();
		glBindVertexArray(loader.rectVAO);
		GLuint framebufferWidth = width_;  // Replace with your desired width
		GLuint framebufferHeight = height_;
		glUniform2f(glGetUniformLocation(loader.framebufferProgram.ID, "resolution"), (float)framebufferWidth, (float)framebufferHeight);
		glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
		glBindTexture(GL_TEXTURE_2D, loader.postProcessingTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);



		ImTextureID myTextureID = (ImTextureID)(intptr_t)loader.framebufferTexture;


		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Load Scene")) {
					std::string file = OpenWindowsFileDialog(L"Scene file (.json)\0*.json\0");
					if (file != "") {
						file = replaceCharacters(file, '\\', '/');
						file = file.substr(51, file.size() - 51);
						LoadNewScene(file.c_str());
					}

				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit")) {
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Window")) {
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Tools")) {
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Build")) {
				if (ImGui::MenuItem("Package Project")) {}
				if (ImGui::MenuItem("Package Game")) {}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Help")) {

				ImGui::Image((void*)(intptr_t)LoadImageTexture("assets/defaults/gui/engine/menus/help/question.png"), ImVec2(16, 16));
				ImGui::SameLine();
				if (ImGui::MenuItem("Wiki")) {
					system("start https://github.com/Windokk/CrabEngine/wiki");
				}
				ImGui::Image((void*)(intptr_t)LoadImageTexture("assets/defaults/gui/engine/menus/help/code.png"), ImVec2(16, 16));
				ImGui::SameLine();
				if (ImGui::MenuItem("Engine Source code")) {
					system("start https://github.com/Windokk/CrabEngine");
				}
				ImGui::Image((void*)(intptr_t)LoadImageTexture("assets/defaults/gui/engine/menus/help/book.png"), ImVec2(16, 16));
				ImGui::SameLine(); // Move to the same line as the image
				if (ImGui::MenuItem("Engine Documentation"))
				{
					system("start https://github.com/Windokk/CrabEngine/blob/master/docs/README.md");
				}

				ImGui::Image((void*)(intptr_t)LoadImageTexture("assets/defaults/gui/engine/menus/help/list.png"), ImVec2(16, 16));
				ImGui::SameLine();
				if (ImGui::MenuItem("Credits")) {
					ImGui::Begin("Credits", nullptr, ImGuiWindowFlags_Popup);
					ImGui::End();
				}
				ImGui::Image((void*)(intptr_t)LoadImageTexture("assets/defaults/gui/engine/menus/help/certificate.png"), ImVec2(16, 16));
				ImGui::SameLine();
				if (ImGui::MenuItem("License")) {
				}
				ImGui::EndMenu();
			}
			// Add more menus here
			ImGui::EndMainMenuBar();
		}

		//Im Gui Viewport
		ImGui::SetNextWindowPos(ImVec2(320, 18));
		ImGui::SetNextWindowSize(ImVec2(640, 395));
		ImGui::Begin("Viewport", nullptr, windowFlags);
		ImGui::Image(myTextureID, ImVec2(640, 360), { 0,1 }, { 1,0 });
		isHoverViewport = (ImGui::IsItemHovered() || ImGui::IsWindowHovered()) && (io.MouseDown[GLFW_MOUSE_BUTTON_LEFT]);
		ImGui::End();

		ImGui::Begin("Outliner", nullptr);
		ImGui::End();

		ImGui::Begin("Details", nullptr);
		ImGui::End();

		ImGui::Begin("Content Browser", nullptr);
		ImGui::End();


		ImGui::EndFrame();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);

		if (isHoverViewport) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
			// Fetches the coordinates of the cursor
			cam.Inputs(window);

		}
		if (!isHoverViewport) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange, ImGuiConfigFlags_DockingEnable;
		}


		// Updates and exports the camera matrix to the Vertex Shader
		cam.updateMatrix(45.0f, 0.1f, 100.0f);

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
