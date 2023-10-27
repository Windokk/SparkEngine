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

struct Transform {
	glm::vec3 Location;
	glm::quat Rotation;
	glm::vec3 Scale;
};

struct Light_Object_Infos {
	int objectID;
	lightType type;
	glm::vec3 lightPos;
	glm::vec3 lightDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;
	float cutOff = glm::cos(glm::radians(12.5f));
	float outerCutOff = glm::cos(glm::radians(15.0f));
	glm::vec3 lightColor;
	float lightIntensity;
	glm::mat4 lightModel;

};

class SceneLoader 
{
public:
	SceneLoader();
	void Load1(const char* loaded_file);
	void Load2();
	void Unload();
	void Update(Camera cam);
	void LoadNewScene(const char* scene);
	void CreateLights();
	void SetLightValues(int objectID, int componentID);
	void SetShadersValues();

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
	std::vector<Light_Object_Infos> light_object_infos;

	//Models
	std::vector<Model> models;

	//Objects positions
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