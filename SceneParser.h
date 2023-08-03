#ifndef SceneParser_CLASS_H
#define SceneParser_CLASS_H


#include"Shader.h"
#include"Utils.h"

#include<glm/glm.hpp>
#include<json/json.h>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<iostream>

using json = nlohmann::json;
using namespace nlohmann;

class SceneParser
{
	public:
		SceneParser(const char* file);
		
		//Shaders data
		struct ShaderData{
			std::string name;
			std::string vert;
			std::string frag;
			std::string geom;
		};
		std::vector<ShaderData> shaders;

		//Lights Data
		struct LightData
		{
			std::string name;
			glm::mat4 light_matrix;
			glm::vec3 light_location;
			glm::quat light_rotation;
			glm::vec3 light_scale;
			float intensity;
			glm::vec4 rgba;
		};
		std::vector<LightData> lights;

		//Models Data
		struct ModelData
		{
			std::string name;
			std::string model_path;
			std::string shader;
			int instancing;
			int type;
			glm::vec3 location;
			glm::quat rotation;
			glm::vec3 scale;
		};
		std::vector<ModelData> models;

		//Meshes Data
		struct MeshData {
			std::string name;
			std::string shape;
			int size;
			std::string shader;
			std::string type;
			glm::vec3 location;
			glm::quat rotation;
			glm::vec3 scale;
			const char* diffuse;
			const char* specular;
		};
		std::vector<MeshData> meshes;

		//Skybox Data
		struct SkyboxData {
			std::string shader;
			std::string right;
			std::string left;
			std::string top;
			std::string bottom;
			std::string front;
			std::string back;
		} skybox;

	private:

		const char* loaded_file = "";
		std::vector<unsigned char> data;
		json JSON;

		std::vector<unsigned char> getData();
};

#endif