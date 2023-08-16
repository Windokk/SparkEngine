#ifndef SceneLoader_CLASS_H
#define SceneLoader_CLASS_H

#include "Model.h"
#include"SceneParser.h"
#include "Utils.h"


#include <json/json.h>



using json = nlohmann::json;

class SceneLoader 
{
public:
	SceneLoader();
	void Load1(const char* loaded_file);
	void Load2();
	void Unload();

	SceneParser parser = SceneParser("");
	int samples;
	int height_;
	int width_;

	//Shaders
	struct Shader_Infos {
		std::string name;
		Shader shader;
	};
	std::vector<Shader_Infos> shaders;

	//Lights
	struct Light_Object_Infos {
		glm::vec4 lightColor;
		glm::vec3 lightPos;
		glm::mat4 lightModel;
		float lightIntensity;
	};
	std::vector<Light_Object_Infos> light_object_infos;

	//Models
	std::vector<Model> models;

	//Objects
	std::vector<std::string> objects;

	//Default Shaders and Framebuffers
	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
	unsigned int cubemapTexture;
	unsigned int rectVAO, rectVBO;
	unsigned int FBO;
	unsigned int framebufferTexture;
	unsigned int postProcessingTexture;
	unsigned int postProcessingFBO;
	Shader skyboxShader;
	Shader framebufferProgram;
	Shader outlineShader;

private:
	const char* file ="";
	std::vector<unsigned char> data;
	json JSON;
};

#endif