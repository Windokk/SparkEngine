//------- Ignore this ----------
#include<filesystem>
namespace fs = std::filesystem;
//------------------------------


//External files
#include<iostream>//<--to print
#include<glad/glad.h>//<--to implement opne gl 
#include<GLFW/glfw3.h>//<-- open gl 3
#include<stb/stb_image.h>//<-- to import images
#include<glm/gtc/matrix_transform.hpp>//<--to create matrices
#include<glm/gtc/type_ptr.hpp>//<-- to create ptr types


//My files
#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"


unsigned int width = 1080;
unsigned int height = 720;


// Pyramid Vertices coordinates
GLfloat pyramid_vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
};

// Pyramid Indices for vertices order
GLuint pyramid_indices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};

// Pyramid Vertices coordinates
GLfloat cube_vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
	 0.0f, 0.0f,  0.0f,     0.0f,  0.0f,  0.0f,	 0.0f, 0.0f,
	 0.0f, 0.0f,  0.5f,     0.0f, 0.0f, 0.0f,	 5.0f, 0.0f,
	 0.5f, 0.0f,  0.5f,     0.0f, 0.0f, 0.0f,	 0.0f, 0.0f,
	 0.5f, 0.0f,  0.0f,     0.0f, 0.0f, 0.0f,	 0.0f, 0.0f,
	 0.0f, 0.5f,  0.0f,     0.0f, 0.0f, 0.0f,	 5.0f, 0.0f,
	 0.0f, 0.5f,  0.5f,     0.0f, 0.0f, 0.0f,	 2.5f, 5.0f,
	 0.5f, 0.5f,  0.5f,     0.0f, 0.0f, 0.0f,	 2.5f, 5.0f,
	 0.5f, 0.5f,  0.0f,     0.92f, 0.0f, 0.0f,	 2.5f, 5.0f
};

// Pyramid Indices for vertices order
GLuint cube_indices[] =
{
	0, 1, 2,
	2, 0, 3,
	0, 4, 1,
	1, 4, 5,
	1, 2, 5,
	5, 2, 6,
	2, 3, 6,
	3, 6, 7,
	3, 7, 0,
	7, 4, 0,
	7, 6, 5,
	4, 7, 5
	
	
};



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
	GLFWwindow* window = glfwCreateWindow(width, height, "Crab Engine", NULL, NULL);

	// Sets the window's icons
	GLFWimage images[2];
	images[0].pixels = stbi_load("assets/defaults/logos/icon.png", &images[0].width, &images[0].height, 0, 4);
	images[1].pixels = stbi_load("assets/defaults/logos/icon.png", &images[1].width, &images[1].height, 0, 4);
	glfwSetWindowIcon(window, 1, images);
	stbi_image_free(images[0].pixels);
	stbi_image_free(images[1].pixels);

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
	// In this case the viewport goes from x = 0, y = 0, to x = width, y = height
	glViewport(0, 0, width, height);

	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");



	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(pyramid_vertices, sizeof(pyramid_vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(pyramid_indices, sizeof(pyramid_indices));

	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 3, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	


	// Texture
	Texture MainTexture("assets/defaults/textures/brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	MainTexture.texUnit(shaderProgram, "tex0", 0);


	glEnable(GL_DEPTH_TEST);


	Camera camera(width, height, glm::vec3(0.0f, 0.4f, 2.0f));

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{


		// Specify the color of the background (black)
		glClearColor(0, 0, 0, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();

		camera.Inputs(window);
		camera.Matrix(45.0f, 0.1f, 100.0f,shaderProgram, "camMatrix");

		// Binds texture so that is appears in rendering
		MainTexture.Bind();
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES,sizeof(pyramid_indices)/sizeof(int), GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	MainTexture.Delete();
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}