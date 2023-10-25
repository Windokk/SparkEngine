#include"SceneParser.h"

SceneParser::SceneParser(const char* file)
{
	if (file != "") {
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
			//We load the objects
			for (unsigned int i = 0; i < JSON["objects"].size(); i++) {
				// i = the object id
				std::string object_name;
				std::vector<std::variant<TransformComponent,ModelComponent,LightComponent>> components;
				/*For every object component 
				      |         |        |
				      V         V        V
				*/
				for (auto& obj_properties : JSON["objects"][i].items()) {
					for (unsigned int comp_id = 0; comp_id < obj_properties.value().size();comp_id++) {
						for (auto& comp_properties : JSON["objects"][i][obj_properties.key()][comp_id]) {
							//obj_properties.key() = object name
							object_name = obj_properties.key();
							for (auto& comp_name : obj_properties.value()[comp_id].items()) {
								//We check if the component is of any of these types
								if (comp_name.key() == "transform") {
									TransformComponent transform;
									glm::vec3 Location;
									glm::quat Rotation;
									glm::vec3 Scale;
									for (auto& param : comp_properties[0].items()) {
										Location = glm::vec3(std::stof((std::string)param.value()[0]), std::stof((std::string)param.value()[1]), std::stof((std::string)param.value()[2]));
									}
									for (auto& param : comp_properties[1].items()) {
										Rotation = glm::quat(std::stof((std::string)param.value()[0]), std::stof((std::string)param.value()[1]), std::stof((std::string)param.value()[2]), std::stof((std::string)param.value()[3]));
									}
									for (auto& param : comp_properties[2].items()) {
										Scale = glm::vec3(std::stof((std::string)param.value()[0]), std::stof((std::string)param.value()[1]), std::stof((std::string)param.value()[2]));
									}
									transform.Location = Location;
									transform.Rotation = Rotation;
									transform.Scale = Scale;
									components.push_back(transform);
								}
								if (comp_name.key() == "model"){
									ModelComponent model;
									std::string model_name;
									std::string model_path;
									std::string shader;
									int instancing;
									for (auto& param : comp_properties[0].items()) {
										model_name = (std::string)param.value();
									}
									for (auto& param : comp_properties[1].items()) {
										model_path = (std::string)param.value();
									}
									for (auto& param : comp_properties[2].items()) {
										shader = (std::string)param.value();
									}
									for (auto& param : comp_properties[3].items()) {
										instancing = std::stoi((std::string)param.value());
									}
									model.model_name = model_name;
									model.model_path = model_path;
									model.shader = shader;
									model.instancing = instancing;
									components.push_back(model);
								}
								if (comp_name.key() == "light") {
									LightComponent light;
									std::string light_name;
									float intensity;
									glm::vec4 color;
									for (auto& param : comp_properties[0].items()) {
										light_name = param.value();
									}
									for (auto& param : comp_properties[1].items()) {
										intensity = std::stof((std::string)param.value());
									}
									for (auto& param : comp_properties[2].items()) {
										color = glm::vec4(std::stof((std::string)param.value()[0]), std::stof((std::string)param.value()[1]), std::stof((std::string)param.value()[2]), std::stof((std::string)param.value()[3]));
									}
									light.light_name = light_name;
									light.intensity = intensity;
									light.color = color;
									components.push_back(light);
								}
							}
						}
					}
				}
				Object obj;
				obj.components = components;
				obj.name = object_name;
				objects.push_back(obj);
			}

		}
	}
}