#ifndef SceneParser_CLASS_H
#define SceneParser_CLASS_H

#include"Shader.h"
#include"EngineUtils.h"
#include"Components.h"

#include<glm/glm.hpp>
#include<json/json.h>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<iostream>
#include <variant>

using json = nlohmann::json;
using namespace nlohmann;

//Shaders data
struct ShaderData {
	std::string name;
	std::string vert;
	std::string frag;
	std::string geom;
};
//Object Data
struct Object {
	std::string name;
	std::vector<std::variant<TransformComponent,ModelComponent,LightComponent>> components;
};

class SceneParser
{
public:
	SceneParser(const char* file);

	//Shaders data
	std::vector<ShaderData> shaders;

	//Skybox Data
	struct SkyboxData {
		std::string shader = "";
		std::string right = "";
		std::string left = "";
		std::string top = "";
		std::string bottom = "";
		std::string front = "";
		std::string back = "";
	} skybox;
	
	std::vector<Object> objects;
	 
private:

	const char* loaded_file = "";
	std::vector<unsigned char> data;
	json JSON;

	std::vector<unsigned char> getData();
};

#endif