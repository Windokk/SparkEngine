#include"SceneParser.h"


SceneParser::SceneParser(const char* file)
{
	if(file!=""){
		// Make a JSON object
		std::string text = get_file_contents(file);
		JSON = json::parse(text);

		SceneParser::loaded_file = file;

		//We load every element
		if (JSON != NULL)
		{
			//We load the shaders
			for (unsigned int i = 0; i < JSON["shaders"].size(); i++) {

				for (auto& x : JSON["shaders"][i].items())
				{
					std::string vert;
					std::string frag;
					std::string geom;

					for (auto& y : JSON["shaders"][i][x.key()][0].items()) {
						vert = (std::string)(y.value());
					}
					for (auto& y : JSON["shaders"][i][x.key()][1].items()) {
						frag = (std::string)(y.value());
					}
					for (auto& y : JSON["shaders"][i][x.key()][2].items()) {
						geom = (std::string)(y.value());
					}

					ShaderData shader_data;	
					shader_data.name = x.key();
					shader_data.vert = vert;
					shader_data.frag = frag;
					shader_data.geom = geom;

					shaders.push_back(shader_data);

				}

			}
			//We load the lights
			for (unsigned int i = 0; i < JSON["lights"].size(); i++) {

				for (auto& x : JSON["lights"][i].items()) {
					std::string light_name;
					glm::mat4 light_matrix{};
					glm::vec3 light_location{};
					glm::quat light_rotation{};
					glm::vec3 light_scale{};
					float intensity{};
					glm::vec4 rgba;

					light_name = x.key();
					for (auto& y : JSON["lights"][i][x.key()][0].items()) {
						light_matrix = (glm::mat4(std::stof((std::string)y.value())));
					}
					for (auto& y : JSON["lights"][i][x.key()][1].items()) {
						light_location = glm::vec3(std::stof((std::string)(y.value()[0])), std::stof((std::string)(y.value()[1])), std::stof((std::string)(y.value()[2])));
					}
					for (auto& y : JSON["lights"][i][x.key()][2].items()) {
						light_rotation = glm::quat(std::stof((std::string)(y.value()[0])), std::stof((std::string)(y.value()[1])), std::stof((std::string)(y.value()[2])), std::stof((std::string)(y.value()[3])));
					}
					for (auto& y : JSON["lights"][i][x.key()][3].items()) {
						light_scale = glm::vec3(std::stof((std::string)(y.value()[0])), std::stof((std::string)(y.value()[1])), std::stof((std::string)(y.value()[2])));
					}
					for (auto& y : JSON["lights"][i][x.key()][4].items()) {
						intensity = std::stof((std::string)(y.value()));
					}
					// we load rgba
					for (auto& y : JSON["lights"][i][x.key()][5].items()) {
						rgba.x = std::stof((std::string)(y.value()[0]));
						rgba.y = std::stof((std::string)(y.value()[1]));
						rgba.z = std::stof((std::string)(y.value()[2]));
						rgba.w = std::stof((std::string)(y.value()[3]));
					}
					
					LightData lightdata;
					lightdata.name = light_name;
					lightdata.light_matrix = light_matrix;
					lightdata.light_location = light_location;
					lightdata.light_rotation = light_rotation;
					lightdata.light_scale = light_scale;
					lightdata.intensity = intensity;
					lightdata.rgba = rgba;
					lights.push_back(lightdata);
				}

			}
			//We load the models
			for (unsigned int i = 0; i < JSON["models"].size(); i++) {
				for (auto& x : JSON["models"][i].items()) {
					std::string model_name;
					std::string model_path{};
					std::string shader{};
					int instancing{};
					int type{};
					glm::vec3 location{};
					glm::quat rotation{};
					glm::vec3 scale{};

					model_name = x.key();
					for (auto& y : JSON["models"][i][x.key()][0].items()) {
						model_path = y.value();
					}
					for (auto& y : JSON["models"][i][x.key()][1].items()) {
						shader = (std::string)y.value();
					}
					for (auto& y : JSON["models"][i][x.key()][2].items()) {
						instancing = stoi((std::string)(y.value()));
					}
					for (auto& y : JSON["models"][i][x.key()][3].items()) {
						type = stoi((std::string)(y.value()));
					}
					for (auto& y : JSON["models"][i][x.key()][4].items()) {
						location = glm::vec3(std::stof((std::string)(y.value()[0])), std::stof((std::string)(y.value()[1])), std::stof((std::string)(y.value()[2])));
					}
					for (auto& y : JSON["models"][i][x.key()][5].items()) {
						rotation = glm::quat(std::stof((std::string)(y.value()[0])), std::stof((std::string)(y.value()[1])), std::stof((std::string)(y.value()[2])), std::stof((std::string)(y.value()[3])));
					}
					for (auto& y : JSON["models"][i][x.key()][6].items()) {
						scale = glm::vec3(std::stof((std::string)(y.value()[0])), std::stof((std::string)(y.value()[1])), std::stof((std::string)(y.value()[2])));
					}

					ModelData modeldata;
					modeldata.name = model_name;
					modeldata.model_path = model_path;
					modeldata.shader = shader;
					modeldata.instancing = instancing;
					modeldata.type = type;
					modeldata.location = location;
					modeldata.rotation = rotation;
					modeldata.scale = scale;

					models.push_back(modeldata);
					
				}
			}
			//We load the meshes
			for (unsigned int i = 0; i < JSON["meshes"].size(); i++) {
				for (auto& x : JSON["meshes"][i].items()) {
					std::string mesh_name{};
					std::string shape{};
					int size{};
					std::string shader{};
					std::string type{};
					glm::vec3 location{};
					glm::quat rotation{};
					glm::vec3 scale{};
					const char* diffuse{};
					const char* specular{};

					mesh_name = x.key();

					for (auto& y : JSON["meshes"][i][x.key()][0].items()) {
						shape = ((std::string)y.value());
					}
					for (auto& y : JSON["meshes"][i][x.key()][1].items()) {
						size = std::stoi((std::string)y.value());
					}
					for (auto& y : JSON["meshes"][i][x.key()][2].items()) {
						shader = (std::string)(y.value());
					}
					for (auto& y : JSON["meshes"][i][x.key()][3].items()) {
						type = (std::string)(y.value());
					}
					for (auto& y : JSON["meshes"][i][x.key()][4].items()) {
						location = glm::vec3(std::stof((std::string)(y.value()[0])), std::stof((std::string)(y.value()[1])), std::stof((std::string)(y.value()[2])));
					}
					for (auto& y : JSON["meshes"][i][x.key()][5].items()) {
						rotation = glm::quat(std::stof((std::string)(y.value()[0])), std::stof((std::string)(y.value()[1])), std::stof((std::string)(y.value()[2])), std::stof((std::string)(y.value()[3])));
					}
					for (auto& y : JSON["meshes"][i][x.key()][6].items()) {
						scale = glm::vec3(std::stof((std::string)(y.value()[0])), std::stof((std::string)(y.value()[1])), std::stof((std::string)(y.value()[2])));
					}
					for (auto& y : JSON["meshes"][i][x.key()][7].items()) {
						diffuse = ((std::string)(y.value())).c_str();
					}
					for (auto& y : JSON["meshes"][i][x.key()][8].items()) {
						specular = ((std::string)(y.value())).c_str();
					}

					MeshData meshdata;
					meshdata.name = mesh_name;
					meshdata.shape = shape;
					meshdata.size = size;
					meshdata.shader = shader;
					meshdata.type = type;
					meshdata.location = location;
					meshdata.rotation = rotation;
					meshdata.scale = scale;
					meshdata.diffuse = diffuse;
					meshdata.specular = specular;

					meshes.push_back(meshdata);
				}
			}
			//We load the skybox
			for (auto& x : JSON["skybox"][0].items()) {
				std::string shader{};
				std::string right{};
				std::string left{};
				std::string top{};
				std::string bottom{};
				std::string front{};
				std::string back{};

				for (auto& y : JSON["skybox"][0][x.key()][0].items()) {
					shader = ((std::string)y.value());
				}
				for (auto& y : JSON["skybox"][0][x.key()][1].items()) {
					right = (std::string)y.value();
				}
				for (auto& y : JSON["skybox"][0][x.key()][2].items()) {
					left = (std::string)y.value();
				}
				for (auto& y : JSON["skybox"][0][x.key()][3].items()) {
					top = (std::string)y.value();
				}
				for (auto& y : JSON["skybox"][0][x.key()][4].items()) {
					bottom = (std::string)y.value();
				}
				for (auto& y : JSON["skybox"][0][x.key()][5].items()) {
					front = (std::string)y.value();
				}
				for (auto& y : JSON["skybox"][0][x.key()][6].items()) {
					back = (std::string)y.value();
				}

				SkyboxData skybox_data;
				skybox_data.shader = shader;
				skybox_data.right = right;
				skybox_data.left = left;
				skybox_data.top = top;
				skybox_data.bottom = bottom;
				skybox_data.front = front;
				skybox_data.back = back;
				skybox = skybox_data;
			}
		}
		
	}
}  