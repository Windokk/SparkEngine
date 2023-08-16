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
#include "srcs/Utils.h"
#include "srcs/imgui/imgui_internal.h"
#include "srcs/Line.h"

unsigned int width_ = 1280;

unsigned int height_ = 720;

unsigned int samples = 8;

int selectedObjectID;

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

void windowclosecallback(GLFWwindow* window) {
	SaveTextureToFile(loader.postProcessingTexture, width_, height_, "./assets/generated/screenshots/texture.png");
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}

struct Ray {
	glm::vec3 origin;
	glm::vec3 direction;
};
bool RayIntersectsMesh(const Ray& ray, const Mesh& mesh) {
	for (size_t i = 0; i < mesh.indices.size(); i += 3) {
		const Vertex& v0 = mesh.vertices[mesh.indices[i]];
		const Vertex& v1 = mesh.vertices[mesh.indices[i + 1]];
		const Vertex& v2 = mesh.vertices[mesh.indices[i + 2]];

		glm::vec3 e1 = v1.position - v0.position;
		glm::vec3 e2 = v2.position - v0.position;
		glm::vec3 h = glm::cross(ray.direction, e2);
		float a = glm::dot(e1, h);

		if (a > -0.00001f && a < 0.00001f)
			continue;

		float f = 1.0f / a;
		glm::vec3 s = ray.origin - v0.position;
		float u = f * glm::dot(s, h);

		if (u < 0.0f || u > 1.0f)
			continue;

		glm::vec3 q = glm::cross(s, e1);
		float v = f * glm::dot(ray.direction, q);

		if (v < 0.0f || u + v > 1.0f)
			continue;

		float t = f * glm::dot(e2, q);

		if (t > 0.0001f) {
			std::cout << "Intersection occurred at triangle " << i << std::endl;
			return true; // Intersection occurred
		}
	}
	return false; // No intersection
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
	bool showCloseButton = true;

	LoadNewScene(current_scene);


	Line debugLine = Line(glm::vec3(0,0,0),glm::vec3(0,-4,10));
	debugLine.setColor(glm::vec3(1, 0, 0));

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

		view = glm::mat4(glm::mat3(glm::lookAt(cam.Position, cam.Position + cam.Orientation, cam.Up)));
		projection = glm::perspective(glm::radians(45.0f), (float)width_ / height_, 0.1f, 100.0f);

		
		debugLine.setMVP(projection * view);
		debugLine.draw();


		glBindFramebuffer(GL_READ_FRAMEBUFFER, loader.FBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, loader.postProcessingFBO);
		// Conclude the multisampling and copy it to the post-processing FBO
		glBlitFramebuffer(0, 0, width_, height_, 0, 0, width_, height_, GL_COLOR_BUFFER_BIT, GL_NEAREST);


		// Bind the default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Draw the framebuffer rectangle
		loader.framebufferProgram.Activate();
		glBindVertexArray(loader.rectVAO);
		float framebufferWidth = width_;  // Replace with your desired width
		float framebufferHeight = height_;
		glUniform2f(glGetUniformLocation(loader.framebufferProgram.ID, "resolution"), framebufferWidth, framebufferHeight);
		glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
		glBindTexture(GL_TEXTURE_2D, loader.postProcessingTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);



		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {

				if (ImGui::BeginMenu("New")) {
					ImGui::Image((void*)(intptr_t)LoadImageTexture("assets/defaults/gui/engine/icons/objects/scene.png"), ImVec2(16, 16));
					ImGui::SameLine();
					if (ImGui::MenuItem("Scene")) {}
					ImGui::Image((void*)(intptr_t)LoadImageTexture("assets/defaults/gui/engine/icons/objects/cube.png"), ImVec2(16, 16));
					ImGui::SameLine();
					if (ImGui::BeginMenu("Object")) {
						ImGui::Image((void*)(intptr_t)LoadImageTexture("assets/defaults/gui/engine/icons/objects/light.png"), ImVec2(16, 16));
						ImGui::SameLine();
						if (ImGui::MenuItem("Light")) {}
						ImGui::Image((void*)(intptr_t)LoadImageTexture("assets/defaults/gui/engine/icons/objects/cube.png"), ImVec2(16, 16));
						ImGui::SameLine();
						if (ImGui::MenuItem("Model")) {}
						ImGui::Image((void*)(intptr_t)LoadImageTexture("assets/defaults/gui/engine/icons/objects/material.png"), ImVec2(16, 16));
						ImGui::SameLine();
						if (ImGui::MenuItem("Shader")) {}
						ImGui::Image((void*)(intptr_t)LoadImageTexture("assets/defaults/gui/engine/icons/objects/script.png"), ImVec2(16, 16));
						ImGui::SameLine();
						if (ImGui::MenuItem("Script")) {}
						ImGui::EndMenu();
					}
					ImGui::Image((void*)(intptr_t)LoadImageTexture("assets/defaults/gui/engine/icons/objects/folder.png"), ImVec2(16, 16));
					ImGui::SameLine();
					if (ImGui::MenuItem("Project")) {}
					ImGui::EndMenu();
				}
				ImGui::Separator();
				if (ImGui::BeginMenu("Import to project")) {
					if (ImGui::MenuItem("Scene")) {
						std::string file = OpenWindowsFileDialog(L"Scene file (.json)\0*.json\0");
						if (file != "") {
							file = replaceCharacters(file, '\\', '/');
							file = file.substr(51, file.size() - 51);
							LoadNewScene(file.c_str());
						}

					}
					if (ImGui::MenuItem("Model")) {

					}
					if (ImGui::MenuItem("Texture")) {
					}
					ImGui::EndMenu();
				}
				ImGui::Spacing();
				ImGui::Spacing();
				if (ImGui::BeginMenu("Save")) {
					if (ImGui::MenuItem("Scene")) {
					}
					if (ImGui::MenuItem("Object")) {

					}
					ImGui::EndMenu();
				}
				ImGui::Spacing();
				ImGui::Spacing();
				if (ImGui::BeginMenu("Open")) {
					if (ImGui::MenuItem("Project")) {}
					ImGui::EndMenu();
				}
				ImGui::Spacing();
				ImGui::Spacing();
				if (ImGui::MenuItem("Quit")) {}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit")) {
				if (ImGui::MenuItem("Select all")) {}
				if (ImGui::MenuItem("Redo")) {}
				if (ImGui::MenuItem("Undo")) {}
				ImGui::Separator();
				if (ImGui::MenuItem("Cut")) {}
				if (ImGui::MenuItem("Copy")) {}
				if (ImGui::MenuItem("Paste")) {}
				if (ImGui::MenuItem("Duplicate")) {}
				if (ImGui::MenuItem("Delete")) {}
				ImGui::Separator();
				if (ImGui::MenuItem("Editor Preferences")) {}
				if (ImGui::MenuItem("Project Settings")) {}

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
		ImGui::Image((void*)(intptr_t)loader.postProcessingTexture, ImVec2(640, 360), { 0,1 }, { 1,0 });
		isHoverViewport = (ImGui::IsItemHovered() || ImGui::IsWindowHovered()) && (io.MouseDown[GLFW_MOUSE_BUTTON_LEFT]);
		ImGui::End();

		ImGui::Begin("Outliner", &showCloseButton);
		ImGui::BeginListBox("Models", ImVec2(200, 80));
		for (int i = 0; i < loader.models.size(); i++) {
			if (ImGui::Selectable((loader.parser.models[i].name).c_str())) {
				for (int j = 0; j < loader.objects.size(); j++) {
					if (loader.objects[j] == loader.parser.models[i].name) {
						selectedObjectID = j;
					}
				}
			}
			ImGui::Separator();
		}
		ImGui::EndListBox();
		ImGui::BeginListBox("Lights", ImVec2(200, 80));
		for (int i = 0; i < loader.parser.lights.size(); i++) {
			if (ImGui::Selectable((loader.parser.lights[i].name).c_str())) {
				for (int j = 0; j < loader.objects.size(); j++) {
					if (loader.objects[j] == loader.parser.lights[i].name) {
						selectedObjectID = j;
					}
				}
			}
			ImGui::Separator();
		}
		ImGui::EndListBox();

		ImGui::End();

		if (ImGui::Begin("Details", &showCloseButton))
			ImGui::End();

		ImGui::Begin("Content Browser", &showCloseButton, (ImGuiWindowFlags_MenuBar));
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("Filters")) {
				ImGui::Image((void*)(intptr_t)LoadImageTexture("assets/defaults/gui/engine/menus/contentbrowser/filter.png"), ImVec2(16, 16));
				ImGui::SameLine();
				ImGui::MenuItem(("Select filter(s) :"), NULL, false, false);
				ImGui::Separator();
				ImGui::EndMenu();
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Save All")) {

			}
			ImGui::Separator();
			ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
			if (ImGui::BeginMenu("Add +")) {
				ImGui::Image((void*)(intptr_t)LoadImageTexture("assets/defaults/gui/engine/icons/objects/scene.png"), ImVec2(16, 16));
				ImGui::SameLine();
				if (ImGui::MenuItem("Scene")) {}
				ImGui::Image((void*)(intptr_t)LoadImageTexture("assets/defaults/gui/engine/icons/objects/cube.png"), ImVec2(16, 16));
				ImGui::SameLine();
				if (ImGui::BeginMenu("Object")) {
					ImGui::Image((void*)(intptr_t)LoadImageTexture("assets/defaults/gui/engine/icons/objects/light.png"), ImVec2(16, 16));
					ImGui::SameLine();
					if (ImGui::MenuItem("Light")) {}
					ImGui::Image((void*)(intptr_t)LoadImageTexture("assets/defaults/gui/engine/icons/objects/cube.png"), ImVec2(16, 16));
					ImGui::SameLine();
					if (ImGui::MenuItem("Model")) {}
					ImGui::Image((void*)(intptr_t)LoadImageTexture("assets/defaults/gui/engine/icons/objects/material.png"), ImVec2(16, 16));
					ImGui::SameLine();
					if (ImGui::MenuItem("Shader")) {}
					ImGui::Image((void*)(intptr_t)LoadImageTexture("assets/defaults/gui/engine/icons/objects/script.png"), ImVec2(16, 16));
					ImGui::SameLine();
					if (ImGui::MenuItem("Script")) {}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			ImGui::PopStyleColor();
			ImGui::EndMenuBar();

		}
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

		std::cout << selectedObjectID << "\n";

		if(isHoverViewport)
		{
			//We check the selection logic
			double mouseX = ImGui::GetMousePos().x;
			double mouseY = ImGui::GetMousePos().y;

			// Convert mouse coordinates to NDC (-1 to 1)
			float ndcX = (2.0f * mouseX) / 800 - 1.0f;
			float ndcY = 1.0f - (2.0f * mouseY) / 600;

			// Create a ray from the camera position through the mouse cursor
			Ray ray;
			ray.origin = cam.Position; // Camera position
			ray.direction = glm::vec3(ndcX, ndcY, -1.0f); // Assume camera looks along negative z


			// Check for ray-box intersection
			bool isMouseOverAirplane = RayIntersectsMesh(ray, loader.models[0].meshes[0]);

			// Print result
			if (isMouseOverAirplane) {
				std::cout << "Mouse is over the model!" << std::endl;
			}
			
		}

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();

	}
	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	debugLine.~Line();
	loader.Unload();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}
