#include"LevelLoader.h"

float rectangleVertices[] = {
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

LevelLoader::LevelLoader() {
}

void LevelLoader::Load1(const char* loaded_file) {
	file = loaded_file;
	parser = LevelParser(file);

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
			infos.shader = Shader((parser.shaders[i].vert).c_str(), (parser.shaders[i].frag).c_str(), "");
			shaders.push_back(infos);
		}

	}

	//Models And Transforms Loading
	for (int i = 0; i < parser.objects.size(); i++) {
		
		for (int a = 0; a < parser.objects[i].components.size(); a++) {
			if (std::holds_alternative<TransformComponent>(parser.objects[i].components[a])) {
				Transform transform;
				transform.Location = std::get<TransformComponent>(parser.objects[i].components[a]).Location;
				transform.Rotation = std::get<TransformComponent>(parser.objects[i].components[a]).Rotation;
				transform.Scale = std::get<TransformComponent>(parser.objects[i].components[a]).Scale;
				objects_Transforms.push_back(transform);
			}
			else if (std::holds_alternative<ModelComponent>(parser.objects[i].components[a])) {
				ModelComponent model = std::get<ModelComponent>(parser.objects[i].components[a]);
				if (model.instancing == 1) {
					models.push_back(Model(model.model_path.c_str(), i));
				}
				
			}
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
	glBindTexture(GL_TEXTURE_2D, framebufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

	// Create Render Buffer Object
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width_, height_);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	// Error checking framebuffer
	int fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer error: " << fboStatus << std::endl;

	//We set the shaders's values
	SetShadersValues();
	//We create the lights
	CreateLights();
	
}

void LevelLoader::Load2() {

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
	if (parser.skybox.top != "") {
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
	/*
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);*/
}

void LevelLoader::Unload() {
	for (int i = 0; i < shaders.size(); i++) {
		shaders[i].shader.Delete();
	}
}

void LevelLoader::Update(Camera cam)
{
	for (int i = 0; i < parser.objects.size(); i++) {
		for (int a = 0; a < parser.objects[i].components.size(); a++) {
			if (std::holds_alternative<ModelComponent>(parser.objects[i].components[a])) {
				ModelComponent model = std::get<ModelComponent>(parser.objects[i].components[a]);
				std::string shader_name = model.shader;
				int shader_id = 0;
				for (int y = 0; y < shaders.size(); y++) {
					if (shaders[y].name == shader_name) {
						shader_id = y;
					}
				}
				for (int x = 0; x < models.size(); x++) {
					if (models[x].model_id == i) {
						std::vector<light_Infos> lights;
						for (int b = 0; b < light_object_infos.size();b++) {
							light_Infos light;
							light.type = light_object_infos[b].type;
							light.position = objects_Transforms[light_object_infos[b].objectID].Location;
							light.direction = light_object_infos[b].lightDirection;
							light.ambient = light_object_infos[b].ambient;
							light.diffuse = light_object_infos[b].diffuse;
							light.specular = light_object_infos[b].specular;
							light.constant = light_object_infos[b].constant;
							light.linear = light_object_infos[b].linear;
							light.quadratic = light_object_infos[b].quadratic;
							light.cutOff = light_object_infos[b].cutOff;
							light.outerCutOff = light_object_infos[b].outerCutOff;
							light.intensity = light_object_infos[b].lightIntensity;
							light.color = light_object_infos[b].lightColor;
							lights.push_back(light); 
						}
						models[x].Draw(shaders[shader_id].shader, cam, objects_Transforms[i].Location, objects_Transforms[i].Rotation, objects_Transforms[i].Scale, lights);
					}
				}
			}
		}
	}
}

void LevelLoader::LoadNewLevel(const char* scene) {

	objects_Transforms.clear();
	models.clear();
	light_object_infos.clear();
	shaders.clear();
	parser.objects.clear();
	parser.shaders.clear();
	parser.skybox = LevelParser::SkyboxData();
	Unload();


	LevelLoader::Load1(scene);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_MULTISAMPLE);

	glEnable(GL_FRAMEBUFFER_SRGB);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	LevelLoader::Load2();
}

void LevelLoader::CreateLights() {
	for (int i = 0; i < parser.objects.size();i++) {
		for (int a = 0; a < parser.objects[i].components.size(); a++) {
			if (std::holds_alternative<LightComponent>(parser.objects[i].components[a])) {
				LightComponent light = std::get<LightComponent>(parser.objects[i].components[a]);
				Light_Object_Infos infos;
				infos.type = light.type;
				infos.lightDirection = light.direction;
				infos.ambient = light.ambient;
				infos.diffuse = light.diffuse;
				infos.specular = light.specular;
				infos.constant = light.constant;
				infos.linear = light.linear;
				infos.quadratic = light.quadratic;
				infos.cutOff = light.cutOff;
				infos.outerCutOff = light.outerCutOff;
				infos.lightIntensity = light.intensity;
				infos.lightColor = light.color;
				infos.lightModel = glm::mat4(1.0f);
				infos.lightModel = glm::translate(infos.lightModel, infos.lightPos);
				infos.objectID = i;
				light_object_infos.push_back(infos);

			}
		}
	}

}

void LevelLoader::SetLightValues(int objectID, int componentID){
	LightComponent lightComponent = std::get<LightComponent>(parser.objects[objectID].components[componentID]);
	Light_Object_Infos infos;
	infos.lightColor = lightComponent.color;
	infos.lightIntensity = lightComponent.intensity;
	infos.lightPos = objects_Transforms[objectID].Location;
	infos.lightModel = glm::mat4(1.0f);
	infos.lightModel = glm::translate(infos.lightModel, infos.lightPos);
	infos.objectID = objectID;
	for (int i = 0; i < light_object_infos.size(); i++) {
		if (light_object_infos[i].objectID = objectID) {
			light_object_infos[i] = infos;
		}
	}
}

void LevelLoader::SetShadersValues() {
	
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	for (int i = 0; i < parser.shaders.size(); i++)
	{
		
		if (shaders[i].name == "skyboxShader")
		{
			skyboxShader = shaders[i].shader;
			shaders[i].shader.Activate();
			glUniform1i(glGetUniformLocation(shaders[i].shader.ID, "skybox"), 0);
		}
		else if (shaders[i].name == "lightShader")
		{
			if (light_object_infos.size() > 0) {
				shaders[i].shader.Activate();
				glUniformMatrix4fv(glGetUniformLocation(shaders[i].shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(light_object_infos[0].lightModel));
				glUniform3f(glGetUniformLocation(shaders[i].shader.ID, "lightColor"), light_object_infos[0].lightColor.x, light_object_infos[0].lightColor.y, light_object_infos[0].lightColor.z);
			}
		}
		else if (shaders[i].name == "framebufferShader") {
			framebufferProgram = shaders[i].shader;
			shaders[i].shader.Activate();
			glUniform1i(glGetUniformLocation(shaders[i].shader.ID, "screenTexture"), 0);
		}
		else if (shaders[i].name.substr(0, 8) == "particle") {
			if (light_object_infos.size() > 0) {
				shaders[i].shader.Activate();
				shaders[i].shader.setVec3("lightColor", light_object_infos[0].lightColor);
				shaders[i].shader.setVec3("lightPos", light_object_infos[0].lightPos);
			}
		}
	}
}
