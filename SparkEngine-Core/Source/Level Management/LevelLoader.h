#include "../Basic Rendering/Model/Model.h"
#include "LevelParser.h"
#include "../Utils/Engine/EngineUtils.h"

#include <variant>
#include <json/json.h>
#include "Components.h"
#include <typeinfo>
#include <Physics/Collision.h>


using json = nlohmann::json;

#ifndef LIGHT_OBJECT_INFOS_H
#define LIGHT_OBJECT_INFOS_H

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

#endif

#ifndef SceneLoader_CLASS_H
#define SceneLoader_CLASS_H
class LevelLoader 
{
public:
	LevelLoader();
	void Load1(const char* loaded_file);
	void Load2();
	void Unload();
	void Update(Camera cam);
	void LoadNewLevel(const char* scene);
	void CreateLights();
	void SetLightValues(int objectID, int componentID);
	void SetShadersValues();

	LevelParser parser = LevelParser("");
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
	unsigned int object_id_FBO;
	unsigned int RBO;
	Shader skyboxShader;
	Shader framebufferProgram;


	//Physics

	std::map<std::string,physics::PhysicsObject> rigidbodies;


private:
	const char* file ="";
	std::vector<unsigned char> data;
	json JSON;
};

#endif