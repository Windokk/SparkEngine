#include"SceneLoader.h"

float rectangleVertices[] =
{
	//  Coords   // texCoords
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f,

	 1.0f,  1.0f,  1.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f
};

float skyboxVertices[] =
{
	//   Coordinates
	-1.0f, -1.0f,  1.0f,//        7--------6
	 1.0f, -1.0f,  1.0f,//       /|       /|
	 1.0f, -1.0f, -1.0f,//      4--------5 |
	-1.0f, -1.0f, -1.0f,//      | |      | |
	-1.0f,  1.0f,  1.0f,//      | 3------|-2
	 1.0f,  1.0f,  1.0f,//      |/       |/
	 1.0f,  1.0f, -1.0f,//      0--------1
	-1.0f,  1.0f, -1.0f
};

unsigned int skyboxIndices[] = {
	// Right
	1, 2, 6,
	6, 5, 1,
	// Left
	0, 4, 7,
	7, 3, 0,
	// Top
	4, 5, 6,
	6, 7, 4,
	// Bottom
	0, 3, 2,
	2, 1, 0,
	// Back
	0, 1, 5,
	5, 4, 0,
	// Front
	3, 7, 6,
	6, 2, 3
};

SceneLoader::SceneLoader() {
}

void SceneLoader::Load1(const char* loaded_file) {
	file = loaded_file;
	parser = SceneParser(file);

	Shader_Infos infos;
	//Shaders loading
	for (int i = 0; i < parser.shaders.size();i++) {
		if ((parser.shaders[i].geom).c_str() != (std::string)"") {
			infos.name = parser.shaders[i].name;
			infos.shader = (Shader((parser.shaders[i].vert).c_str(), (parser.shaders[i].frag).c_str(), (parser.shaders[i].geom).c_str()));
			
			shaders.push_back(infos);
		}
		else {
			infos.name = parser.shaders[i].name;
			infos.shader = (Shader((parser.shaders[i].vert).c_str(), (parser.shaders[i].frag).c_str(), ""));
			shaders.push_back(infos);
		}
		
	}

	//Base Models loading
	for (int i = 0; i < parser.models.size(); i++) {
		if (parser.models[i].instancing == 1) {
			models.push_back(Model((parser.models[i].model_path).c_str()));
		}
	}

	// Prepare framebuffer rectangle VBO and VAO
	glGenVertexArrays(1, &rectVAO);
	glGenBuffers(1, &rectVBO);
	glBindVertexArray(rectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// Create Frame Buffer Object
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Create Framebuffer Texture
	glGenTextures(1, &framebufferTexture);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, framebufferTexture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width_, height_, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, framebufferTexture, 0);

	// Create Render Buffer Object
	unsigned int RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width_, height_);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);


	// Error checking framebuffer
	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer error: " << fboStatus << std::endl;

	// Create Frame Buffer Object
	glGenFramebuffers(1, &postProcessingFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, postProcessingFBO);

	// Create Framebuffer Texture
	glGenTextures(1, &postProcessingTexture);
	glBindTexture(GL_TEXTURE_2D, postProcessingTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postProcessingTexture, 0);

	// Error checking framebuffer
	fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Post-Processing Framebuffer error: " << fboStatus << std::endl;

	//We create the lights
	for (int i = 0; i < parser.lights.size(); i++) {
		Light_Object_Infos infos;
		infos.lightColor = parser.lights[i].rgba;
		infos.lightIntensity = parser.lights[i].intensity;
		infos.lightPos = parser.lights[i].light_location;
		infos.lightModel = parser.lights[i].light_matrix;
		infos.lightModel = glm::translate(infos.lightModel, infos.lightPos);
		light_object_infos.push_back(infos);
	}

	glm::mat4 objectModel =  glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	//We set the shaders's values
	for (int i = 0; i < parser.shaders.size(); i++)
	{
		if (parser.shaders[i].name == "defaultShader") {
			
			if (light_object_infos.size()>0) {
				shaders[i].shader.Activate();
				glUniformMatrix4fv(glGetUniformLocation(shaders[i].shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
				glUniform4f(glGetUniformLocation(shaders[i].shader.ID, "lightColor"), light_object_infos[0].lightColor.x, light_object_infos[0].lightColor.y, light_object_infos[0].lightColor.z, light_object_infos[0].lightColor.w);
				glUniform3f(glGetUniformLocation(shaders[i].shader.ID, "lightPos"), light_object_infos[0].lightPos.x, light_object_infos[0].lightPos.y, light_object_infos[0].lightPos.z);
				glUniform1f(glGetUniformLocation(shaders[i].shader.ID, "light_intensity"), light_object_infos[0].lightIntensity);
			}
			
		}
		else if(parser.shaders[i].name == "skyboxShader")
		{
			skyboxShader = shaders[i].shader;
			shaders[i].shader.Activate();
			glUniform1i(glGetUniformLocation(shaders[i].shader.ID, "skybox"), 0);
		}
		else if(parser.shaders[i].name == "lightShader")
		{
			if (light_object_infos.size() > 0) {
				shaders[i].shader.Activate();
				glUniformMatrix4fv(glGetUniformLocation(shaders[i].shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(light_object_infos[0].lightModel));
				glUniform4f(glGetUniformLocation(shaders[i].shader.ID, "lightColor"), light_object_infos[0].lightColor.x, light_object_infos[0].lightColor.y, light_object_infos[0].lightColor.z, light_object_infos[0].lightColor.w);
			}
		}
		else if (parser.shaders[i].name == "framebufferShader") {
			framebufferProgram = shaders[i].shader;
			shaders[i].shader.Activate();
			glUniform1i(glGetUniformLocation(shaders[i].shader.ID, "screenTexture"), 0);
		}
		else if(parser.shaders[i].name.substr(0,8) == "particle") {
			if (light_object_infos.size() > 0) {
				shaders[i].shader.Activate();
				glUniform4f(glGetUniformLocation(shaders[i].shader.ID, "lightColor"), light_object_infos[0].lightColor.x, light_object_infos[0].lightColor.y, light_object_infos[0].lightColor.z, light_object_infos[0].lightColor.w);
				glUniform3f(glGetUniformLocation(shaders[i].shader.ID, "lightPos"), light_object_infos[0].lightPos.x, light_object_infos[0].lightPos.y, light_object_infos[0].lightPos.z);
			}
		}
	}

	
	
}

void SceneLoader::Load2() {
	
	//We do the instancing logic
	for (int i = 0; i < parser.models.size(); i++) {
			if (parser.models[i].instancing != 1) {
				// The number of asteroids to be created
				const unsigned int number = parser.models[i].instancing;

				const char* path = (parser.models[i].model_path).c_str();

				// Radius of circle around which asteroids orbit
				float radius = 100.0f;
				// How much ateroids deviate from the radius
				float radiusDeviation = 25.0f;

				// Holds all transformations for the asteroids
				std::vector <glm::mat4> instanceMatrix;

				for (unsigned int a = 0; a < number; a++)
				{
					// Generates x and y for the function x^2 + y^2 = radius^2 which is a circle
					float x = randf();
					float finalRadius = radius + randf() * radiusDeviation;
					float y = ((rand() % 2) * 2 - 1) * sqrt(1.0f - x * x);

					// Holds transformations before multiplying them
					glm::vec3 tempTranslation;
					glm::quat tempRotation;
					glm::vec3 tempScale;

					// Makes the random distribution more even
					if (randf() > 0.5f)
					{
						// Generates a translation near a circle of radius "radius"
						tempTranslation = glm::vec3(y * finalRadius, randf(), x * finalRadius);
					}
					else
					{
						// Generates a translation near a circle of radius "radius"
						tempTranslation = glm::vec3(x * finalRadius, randf(), y * finalRadius);
					}
					// Generates random rotations
					tempRotation = glm::quat(1.0f, randf(), randf(), randf());
					// Generates random scales
					tempScale = 0.1f * glm::vec3(randf(), randf(), randf());


					// Initialize matrices
					glm::mat4 trans = glm::mat4(1.0f);
					glm::mat4 rot = glm::mat4(1.0f);
					glm::mat4 sca = glm::mat4(1.0f);

					// Transform the matrices to their correct form
					trans = glm::translate(trans, tempTranslation);
					rot = glm::mat4_cast(tempRotation);
					sca = glm::scale(sca, tempScale);

					// Push matrix transformation
					instanceMatrix.push_back(trans * rot * sca);
				}
				models.push_back(Model(path, number, instanceMatrix));
			}
			
		}

		//We load the skybox
		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);
		glGenBuffers(1, &skyboxEBO);
		glBindVertexArray(skyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


		std::string facesCubemap[6] =
		{
			parser.skybox.right,
			parser.skybox.left,
			parser.skybox.top,
			parser.skybox.bottom,
			parser.skybox.front,
			parser.skybox.back
		};

		// Creates the cubemap texture object
		glGenTextures(1, &cubemapTexture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// These are very important to prevent seams
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		// This might help with seams on some systems
		//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

			// Cycles through all the textures and attaches them to the cubemap object
		for (unsigned int i = 0; i < 6; i++)
		{
			int width, height, nrChannels;
			unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				stbi_set_flip_vertically_on_load(false);
				glTexImage2D
				(
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0,
					GL_RGB,
					width,
					height,
					0,
					GL_RGB,
					GL_UNSIGNED_BYTE,
					data
				);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
				stbi_image_free(data);
			}
		}
}

void SceneLoader::Unload() {
	for (int i = 0; i < shaders.size(); i++) {
		shaders[i].shader.Delete();
	}
}