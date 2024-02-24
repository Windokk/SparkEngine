#include <iostream>
#include <glad/glad.h>
#include <windows.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Libraries/ImGui_Lib/imgui_internal.h"

#include "Level Management/LevelLoader.h"

#include "Gui/ImGuiMain.h"


#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_NATIVE_INCLUDE_NONE

#include <GLFW/glfw3native.h>

bool can_test_for_stop = false;

unsigned int width_ = 1280;
unsigned int height_ = 720;

unsigned int anti_aliasing_samples = 8;

int selectedObjectID = -1;

LevelLoader loader;

int file_refresh_timer = 0;

ImGuiMain* gui = new ImGuiMain();

const char* current_level = "../SparkEngine-Core/assets/defaults/levels/test_simple_light.sl";

Camera cam = Camera(0, 0, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));

EditorPlayer player = EditorPlayer();

PathTracingInfos path_tracing_infos;

void RenderLevel() {
	glBindFramebuffer(GL_FRAMEBUFFER, loader.FBO);

	// Specify the color of the background
	glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
	// Clean the back buffer and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	// Enable depth testing since it's disabled when drawing the framebuffer rectangle
	glEnable(GL_DEPTH_TEST);

	//Update the level from the loader
	loader.Update(cam, &player.isPlaying, path_tracing_infos);

	glDepthFunc(GL_LEQUAL);

	loader.skyboxShader.Activate();
	glm::mat4 view = glm::mat3(cam.view);
	glm::mat4 projection = cam.projection;
	loader.skyboxShader.setMat4("view", view);
	loader.skyboxShader.setMat4("projection", projection);

	// Draws the cubemap as the last object so we can save a bit of performance by discarding all fragments
	// where an object is present (a depth of 1.0f will always fail against any object's depth value)
	glBindVertexArray(loader.skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, loader.cubemapTexture);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	// Switch back to the normal depth function
	glDepthFunc(GL_LESS);

	// Bind the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Draw the framebuffer rectangle
	loader.framebufferProgram.Activate();
	glBindVertexArray(loader.rectVAO);
	glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
	glBindTexture(GL_TEXTURE_2D, loader.framebufferTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
/////													   /////
/////		Here's the code to make the window			   /////
/////              without a title bar					   /////
/////													   /////
/////		V               V                V		       /////
/////													   /////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

WNDPROC original_proc;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_NCCALCSIZE:
	{
		// Remove the window's standard sizing border
		if (wParam == TRUE && lParam != NULL)
		{
			NCCALCSIZE_PARAMS* pParams = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
			pParams->rgrc[0].top += 1;
			pParams->rgrc[0].right -= 2;
			pParams->rgrc[0].bottom -= 2;
			pParams->rgrc[0].left += 2;
		}
		return 0;
	}
	case WM_NCPAINT:
	{
		// Prevent the non-client area from being painted
		return 0;
	}
	case WM_NCHITTEST:
	{
		// Expand the hit test area for resizing
		const int borderWidth = 8; // Adjust this value to control the hit test area size

		POINTS mousePos = MAKEPOINTS(lParam);
		POINT clientMousePos = { mousePos.x, mousePos.y };
		ScreenToClient(hWnd, &clientMousePos);

		RECT windowRect;
		GetClientRect(hWnd, &windowRect);

		if (clientMousePos.y >= windowRect.bottom - borderWidth)
		{
			if (clientMousePos.x <= borderWidth)
				return HTBOTTOMLEFT;
			else if (clientMousePos.x >= windowRect.right - borderWidth)
				return HTBOTTOMRIGHT;
			else
				return HTBOTTOM;
		}
		else if (clientMousePos.y <= borderWidth)
		{
			if (clientMousePos.x <= borderWidth)
				return HTTOPLEFT;
			else if (clientMousePos.x >= windowRect.right - borderWidth)
				return HTTOPRIGHT;
			else
				return HTTOP;
		}
		else if (clientMousePos.x <= borderWidth)
		{
			return HTLEFT;
		}
		else if (clientMousePos.x >= windowRect.right - borderWidth)
		{
			return HTRIGHT;
		}

		break;
	}
	}

	return CallWindowProc(original_proc, hWnd, uMsg, wParam, lParam);
}

void disableTitlebar(GLFWwindow* window)
{
	HWND hWnd = glfwGetWin32Window(window);

	LONG_PTR lStyle = GetWindowLongPtr(hWnd, GWL_STYLE);
	lStyle |= WS_THICKFRAME;
	lStyle &= ~WS_CAPTION;
	SetWindowLongPtr(hWnd, GWL_STYLE, lStyle);

	RECT windowRect;
	GetWindowRect(hWnd, &windowRect);
	int width = windowRect.right - windowRect.left;
	int height = windowRect.bottom - windowRect.top;

	original_proc = (WNDPROC)GetWindowLongPtr(hWnd, GWLP_WNDPROC);
	(WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProc));
	SetWindowPos(hWnd, NULL, 0, 0, width, height, SWP_FRAMECHANGED | SWP_NOMOVE);
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

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "Spark Engine"
	GLFWwindow* window = glfwCreateWindow(width_, height_, "Spark Engine", NULL, NULL);
	
	GLFWimage images[2];
	images[0].pixels = stbi_load("../SparkEngine-Core/assets/defaults/logos/icon.png", &images[0].width, &images[0].height, 0, 4);
	images[1].pixels = stbi_load("../SparkEngine-Core/assets/defaults/logos/icon.png", &images[1].width, &images[1].height, 0, 4);
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

	//Creates a loader object to load levels
	loader.width_ = width_;
	loader.height_ = height_;
	loader.anti_aliasing_samples = anti_aliasing_samples;

	// Creates camera object
	cam = Camera(width_, height_, glm::vec3(36.855, 16.3166, -46.2548), glm::vec3(-0.581428, -0.200356, 0.788543));

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
	gui->Load(window, io);

	loader.LoadNewLevel(current_level);

	int frameCounter = 0;


	path_tracing_infos.resolution = glm::vec2(gui->viewportSize.x, gui->viewportSize.y);
	path_tracing_infos.invNumTiles = glm::vec2((float)1280 / 100, (float)720 / 100);
	path_tracing_infos.numOfLights = loader.light_object_infos.size();
	path_tracing_infos.accumTexture = 0;
	path_tracing_infos.BVH = 1;
	path_tracing_infos.vertexIndicesTex = 2;
	path_tracing_infos.verticesTex = 3;
	path_tracing_infos.normalsTex = 4;
	path_tracing_infos.materialsTex = 5;
	path_tracing_infos.transformsTex = 6;
	path_tracing_infos.lightsTex = 7;
	path_tracing_infos.textureMapsArrayTex = 8;
	path_tracing_infos.envMapTex = 9;
	path_tracing_infos.envMapCDFTex = 10;


	while (!glfwWindowShouldClose(window)) {

		frameCounter++;

		RenderLevel();

		glm::mat4 view = glm::mat4(glm::lookAt(cam.Position, cam.Position + cam.Orientation, cam.Up));
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width_ / height_, 0.1f, 100.0f);

		//We draw the ImGui interface
		gui->Draw(window, cam, loader, selectedObjectID, io, player);

		//We don't refresh content browser every tick, only every 200 ticks
		file_refresh_timer++;
		if (file_refresh_timer / 200 == 1) {
			file_refresh_timer = 0;
			gui->manager.files = ListFiles((char*)gui->current_dir);
		}

		//Updates cam width and height if the viewport's size is changed
		if (gui->viewportSize.x != cam.width or gui->viewportSize.y != cam.height) {
			cam.updateSize(gui->viewportSize.x, gui->viewportSize.y);
		}

		//Screenshots
		if (io.KeysDown[GLFW_KEY_F9]) {
			SaveTextureToFile(loader.framebufferTexture, width_, height_, "assets/generated/screenshots/texture.png", *gui);
		}


		/////////////////////////////////////////////////////
		/////////////////////////////////////////////////////
		/////////////////////////////////////////////////////
		////											 ////
		////		Here we run the editor play		     ////
		////											 ////
		////										     ////
		/////////////////////////////////////////////////////
		/////////////////////////////////////////////////////
		/////////////////////////////////////////////////////

		if (gui->play && not player.isPlaying) {
			player.Prepare(&loader);
			selectedObjectID = -1;
		}

		if (gui->play && player.isPlaying && can_test_for_stop) {
			player.Stop(&loader);
			can_test_for_stop = false;
			gui->play = false;
		}

		if (player.isPlaying) {
			player.Play();
			can_test_for_stop = true;
			gui->play = false;
		}

		
		path_tracing_infos.camera_position = cam.Position;
		path_tracing_infos.camera_right = glm::vec3(1.0f, 0.0f, 0.0f);
		path_tracing_infos.camera_up = cam.Up;
		path_tracing_infos.camera_forward = glm::vec3(0.0f, 0.0f, 1.0f);
		path_tracing_infos.camera_fov = cam.FOVdeg;
		path_tracing_infos.camera_focalDist = 0.1f;
		path_tracing_infos.camera_aperture = 0.0f;
		path_tracing_infos.envMapIntensity = 0;
		path_tracing_infos.envMapRot = 0;
		path_tracing_infos.maxDepth = 2;
		path_tracing_infos.tileOffset = glm::vec2((float)-1 * 100 / 1280, (float)-1 * 100 / 720);
		path_tracing_infos.uniformLightCol = glm::vec3(255,255,255);
		path_tracing_infos.roughnessMollificationAmt = 0.0f;
		path_tracing_infos.frameNum = frameCounter;
		


		// Updates and exports the camera matrix to the Vertex Shader
		cam.updateMatrix(45.0f, 0.1f, 1000.0f);

		glfwSwapInterval(0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	//We shutdown ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	loader.Unload();
	//next line is optional: it will be cleared by the destructor when the array goes out of scope
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}