#include"LevelParser.h"

LevelParser::LevelParser(const char* file)
{
	if (file != "") {
		// Make a JSON object
		std::string text = get_file_contents(file);
		JSON = json::parse(text);

		LevelParser::loaded_file = file;

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
			std::string shader{};
			std::string right{};
			std::string left{};
			std::string top{};
			std::string bottom{};
			std::string front{};
			std::string back{};

			for (auto& y : JSON["skybox"][0].items()) {
				shader = ((std::string)y.value());
			}
			for (auto& y : JSON["skybox"][1].items()) {
				right = (std::string)y.value();
			}
			for (auto& y : JSON["skybox"][2].items()) {
				left = (std::string)y.value();
			}
			for (auto& y : JSON["skybox"][3].items()) {
				top = (std::string)y.value();
			}
			for (auto& y : JSON["skybox"][4].items()) {
				bottom = (std::string)y.value();
			}
			for (auto& y : JSON["skybox"][5].items()) {
				front = (std::string)y.value();
			}
			for (auto& y : JSON["skybox"][6].items()) {
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
				
			if (!JSON["objects"].is_null()) {
				//We load the objects
				for (unsigned int i = 0; i < JSON["objects"].size(); i++) {
					// i = the object id
					std::string object_name;
					std::vector<std::variant<TransformComponent,
						ModelComponent,
						LightComponent,
						SphereColliderComponent,
						PlaneColliderComponent,
						RigidbodyComponent>> components;
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
										lightType type;
										glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
										glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);
										glm::vec3 ambient;
										glm::vec3 diffuse;
										glm::vec3 specular;
										float constant = 1.0f;
										float linear = 0.09f;
										float quadratic = 0.032f;
										float cutOff = glm::cos(glm::radians(12.5f));
										float outerCutOff = glm::cos(glm::radians(15.0f));
										float intensity;
										glm::vec3 color;
										//Type
										for (auto& param : comp_properties[0].items()) {
											std::string type_str= (std::string)param.value();
											if (type_str == "directionnal") {
												type = LT_DIRECTIONNAL;
											}
											else if (type_str == "point") {
												type = LT_POINT;
											}
											else if (type_str == "spot") {
												type = LT_SPOT;
											}
										}
										switch (type) {
										case LT_DIRECTIONNAL:
											//Direction
											for (auto& param : comp_properties[1].items()) {
												direction = glm::vec3(std::stof((std::string)param.value()[0]), std::stof((std::string)param.value()[1]), std::stof((std::string)param.value()[2]));
											}
											//Ambient
											for (auto& param : comp_properties[2].items()) {
												ambient = glm::vec3(std::stof((std::string)param.value()[0]), std::stof((std::string)param.value()[1]), std::stof((std::string)param.value()[2]));
											}
											//Diffuse
											for (auto& param : comp_properties[3].items()) {
												diffuse = glm::vec3(std::stof((std::string)param.value()[0]), std::stof((std::string)param.value()[1]), std::stof((std::string)param.value()[2]));
											}
											//Specular
											for (auto& param : comp_properties[4].items()) {
												specular = glm::vec3(std::stof((std::string)param.value()[0]), std::stof((std::string)param.value()[1]), std::stof((std::string)param.value()[2]));
											}
											//Intensity
											for (auto& param : comp_properties[5].items()) {
												intensity = std::stof((std::string)param.value());
											}
											//Color
											for (auto& param : comp_properties[6].items()) {
												color = glm::vec3(std::stof((std::string)param.value()[0]), std::stof((std::string)param.value()[1]), std::stof((std::string)param.value()[2]));
											}
											break;
										case LT_POINT:
											//Ambient
											for (auto& param : comp_properties[1].items()) {
												ambient = glm::vec3(std::stof((std::string)param.value()[0]), std::stof((std::string)param.value()[1]), std::stof((std::string)param.value()[2]));
											}
											//Diffuse
											for (auto& param : comp_properties[2].items()) {
												diffuse = glm::vec3(std::stof((std::string)param.value()[0]), std::stof((std::string)param.value()[1]), std::stof((std::string)param.value()[2]));
											}
											//Specular
											for (auto& param : comp_properties[3].items()) {
												specular = glm::vec3(std::stof((std::string)param.value()[0]), std::stof((std::string)param.value()[1]), std::stof((std::string)param.value()[2]));
											}
											//Constant
											for (auto& param : comp_properties[4].items()) {
												constant = std::stof((std::string)param.value());
											}
											//Linear
											for (auto& param : comp_properties[5].items()) {
												linear = std::stof((std::string)param.value());
											}
											//Quadratic
											for (auto& param : comp_properties[6].items()) {
												quadratic = std::stof((std::string)param.value());
											}
											//Intensity
											for (auto& param : comp_properties[7].items()) {
												intensity = std::stof((std::string)param.value());
											}
											//Color
											for (auto& param : comp_properties[8].items()) {
												color = glm::vec3(std::stof((std::string)param.value()[0]), std::stof((std::string)param.value()[1]), std::stof((std::string)param.value()[2]));
											}
											break;
										case LT_SPOT:
											//Direction
											for (auto& param : comp_properties[1].items()) {
												direction = glm::vec3(std::stof((std::string)param.value()[0]), std::stof((std::string)param.value()[1]), std::stof((std::string)param.value()[2]));
											}
											//Ambient
											for (auto& param : comp_properties[2].items()) {
												ambient = glm::vec3(std::stof((std::string)param.value()[0]), std::stof((std::string)param.value()[1]), std::stof((std::string)param.value()[2]));
											}
											//Diffuse
											for (auto& param : comp_properties[3].items()) {
												diffuse = glm::vec3(std::stof((std::string)param.value()[0]), std::stof((std::string)param.value()[1]), std::stof((std::string)param.value()[2]));
											}
											//Specular
											for (auto& param : comp_properties[4].items()) {
												specular = glm::vec3(std::stof((std::string)param.value()[0]), std::stof((std::string)param.value()[1]), std::stof((std::string)param.value()[2]));
											}
											//Constant
											for (auto& param : comp_properties[5].items()) {
												constant = std::stof((std::string)param.value());
											}
											//Linear
											for (auto& param : comp_properties[6].items()) {
												linear = std::stof((std::string)param.value());
											}
											//Quadratic
											for (auto& param : comp_properties[7].items()) {
												quadratic = std::stof((std::string)param.value());
											}
											//CutOff
											for (auto& param : comp_properties[8].items()) {
												cutOff = glm::cos(glm::radians(std::stof((std::string)param.value())));
											}
											//OuterCutOff
											for (auto& param : comp_properties[9].items()) {
												outerCutOff = glm::cos(glm::radians(std::stof((std::string)param.value())));
											}
											//Intensity
											for (auto& param : comp_properties[10].items()) {
												intensity = std::stof((std::string)param.value());
											}
											//Color
											for (auto& param : comp_properties[11].items()) {
												color = glm::vec3(std::stof((std::string)param.value()[0]), std::stof((std::string)param.value()[1]), std::stof((std::string)param.value()[2]));
											}
											break;
										}
										light.type = type;
										light.direction = direction;
										light.ambient = ambient;
										light.diffuse = diffuse;
										light.specular = specular;
										light.constant = constant;
										light.linear = linear;
										light.quadratic = quadratic;
										light.cutOff = cutOff;
										light.outerCutOff = outerCutOff;
										light.intensity = intensity;
										light.color = color;
										components.push_back(light);
									}
									if (comp_name.key() == "rigidbody"){
										RigidbodyComponent rigidbody;
										float mass;
										ColliderComponent collider;
										for (auto& param : comp_properties[0].items()) {
											mass = std::stof((std::string)param.value());
										}
										for (auto& param : comp_properties[1].items()) {
											std::string type = (std::string)param.value();
											if (type == "sphere") {
												for (const auto& variant : components) {
													collider = *std::get_if<SphereColliderComponent>(&variant);
												}
											}

											if (type == "plane") {
												for (const auto& variant : components) {
													collider = *std::get_if<PlaneColliderComponent>(&variant);
												}
											}
										}
										rigidbody.mass = mass;
										rigidbody.collider = collider;
										components.push_back(rigidbody);
									}
									if (comp_name.key() == "sphere_collider"){
										SphereColliderComponent sphere_collider;
										glm::vec3 center;
										float radius;
										for (auto& param : comp_properties[0].items()) {
											radius = std::stof((std::string)param.value());
										}
										for (auto& param : comp_properties[1].items()) {
											center = glm::vec3(std::stof((std::string)param.value()[0]), std::stof((std::string)param.value()[1]), std::stof((std::string)param.value()[2]));
										}  
									}
									if (comp_name.key() == "plane_collider"){
										glm::vec3 normal;
										float distance;
										
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
}