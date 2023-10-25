#ifndef SceneLoader_CLASS_H
#define SceneLoader_CLASS_H

#include "Model.h"
#include"SceneParser.h"
#include "EngineUtils.h"

#include <variant>
#include <json/json.h>
#include "Components.h"
#include <typeinfo>


using json = nlohmann::json;

class SceneLoader 
{
public:
	SceneLoader();
	void Load1(const char* loaded_file);
	void Load2();
	void Unload();
	void Update(Camera cam);
	void LoadNewScene(const char* scene);

	SceneParser parser = SceneParser("");
	int anti_aliasing_samples;
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

	//Objects positions
	struct Transform {
		glm::vec3 Location;
		glm::quat Rotation;
		glm::vec3 Scale;
	};
	std::vector<Transform> objects_Transforms;

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

private:
	const char* file ="";
	std::vector<unsigned char> data;
	json JSON;
};

#endif