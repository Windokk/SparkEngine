//------- Ignore this ----------
#include<filesystem>
namespace fs = std::filesystem;
//------------------------------

#include"Model.h"
#include"Texture.h"


unsigned int width_ = 800;
unsigned int height_ = 800;

void window_size_callback(GLFWwindow* window, int width, int height)
{
	width_ = width;
	height_ = height;
	glViewport(0, 0, width_, height_);
}


int WinMain()
{
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

	glfwSetWindowSizeCallback(window, window_size_callback);

	// Error check if the window fails to create
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
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width_, height_);





	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");
	// Shader for the outlining model
	Shader outliningProgram("outlining.vert", "outlining.frag");

	// Take care of all the light related things
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);



	

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

	// Creates camera object
	Camera camera(width_, height_, glm::vec3(0.0f, 0.0f, 2.0f));
	
	// Load in models
	Model model("assets/defaults/models/crow/scene.gltf");

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f);



		// Make it so the stencil test always passes
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		// Enable modifying of the stencil buffer
		glStencilMask(0xFF);
		// Draw the normal model
		model.Draw(shaderProgram, camera);

		// Make it so only the pixels without the value 1 pass the test
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		// Disable modifying of the stencil buffer
		glStencilMask(0x00);
		// Disable the depth buffer
		glDisable(GL_DEPTH_TEST);

		// Enable modifying of the stencil buffer
		glStencilMask(0xFF);
		// Clear stencil buffer
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		// Enable the depth buffer
		glEnable(GL_DEPTH_TEST);



		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}