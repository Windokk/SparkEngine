#include "SceneWriter.h"

SceneWriter::SceneWriter() {

}

void SceneWriter::WriteEmptySceneToFile(const char* filename)
{
	// We initialize the json data
	json data;

	// We fill the json data
	data["shaders"];

	data["shaders"][0]["defaultShader"][0]["vert"] = "./srcs/Shaders/Defaults/Engine/default.vert";
	data["shaders"][0]["defaultShader"][1]["frag"] = "./srcs/Shaders/Defaults/Engine/default.frag";
	data["shaders"][0]["defaultShader"][2]["geom"] = "";

	data["shaders"][1]["skyboxShader"][0]["vert"] = "./srcs/Shaders/Defaults/Engine/Skyboxes/skybox.vert";
	data["shaders"][1]["skyboxShader"][1]["frag"] = "./srcs/Shaders/Defaults/Engine/Skyboxes/skybox.frag";
	data["shaders"][1]["skyboxShader"][2]["geom"] = "";

	data["shaders"][2]["framebufferShader"][0]["vert"] = "./srcs/Shaders/Defaults/Engine/Framebuffer/framebuffer.vert";
	data["shaders"][2]["framebufferShader"][1]["frag"] = "./srcs/Shaders/Defaults/Engine/Framebuffer/framebuffer.frag";
	data["shaders"][2]["framebufferShader"][2]["geom"] = "";

	data["skybox"][0]["shader"] = "skyboxShader";
	data["skybox"][1]["right"] = "./assets/defaults/textures/skyboxes/base/blue.png";
	data["skybox"][2]["left"] = "./assets/defaults/textures/skyboxes/base/blue.png";
	data["skybox"][3]["top"] = "./assets/defaults/textures/skyboxes/base/blue.png";
	data["skybox"][4]["bottom"] = "./assets/defaults/textures/skyboxes/base/blue.png";
	data["skybox"][5]["front"] = "./assets/defaults/textures/skyboxes/base/blue.png";
	data["skybox"][6]["back"] = "./assets/defaults/textures/skyboxes/base/blue.png";
	

	// We write the data and export the file
	std::string jsonString = data.dump(4);

	std::ofstream outputFile;

	outputFile.open(filename);
	outputFile << jsonString;
	outputFile.close();
}

void SceneWriter::WriteSceneToFile(const char* filename, SceneLoader& loader)
{
	// We initialize the json data
	json data;

	// We fill the json data
	for (int i = 0; i < loader.parser.shaders.size(); i++) {
		data["shaders"][i][loader.parser.shaders[i].name][0]["vert"] = loader.parser.shaders[i].vert;
		data["shaders"][i][loader.parser.shaders[i].name][1]["frag"] = loader.parser.shaders[i].frag;
		data["shaders"][i][loader.parser.shaders[i].name][2]["geom"] = loader.parser.shaders[i].geom;
	}
	
	data["skybox"][0]["shader"] = loader.parser.skybox.shader;
	data["skybox"][1]["right"] = loader.parser.skybox.right;
	data["skybox"][2]["left"] = loader.parser.skybox.left;
	data["skybox"][3]["top"] = loader.parser.skybox.top;
	data["skybox"][4]["bottom"] = loader.parser.skybox.bottom;
	data["skybox"][5]["front"] = loader.parser.skybox.front;
	data["skybox"][6]["back"] = loader.parser.skybox.back;


	for (int i = 0; i < loader.parser.objects.size(); i++) {
		for (int x = 0; x < loader.parser.objects[i].components.size();x++) {
			if (std::holds_alternative<TransformComponent>(loader.parser.objects[i].components[x])) {
				//Location 
				data["objects"][i][loader.parser.objects[i].name][x]["transform"][0]["location"][0] = std::to_string(loader.parser.objects[i].GetComponent<TransformComponent>().Location.x);
				data["objects"][i][loader.parser.objects[i].name][x]["transform"][0]["location"][1] = std::to_string(loader.parser.objects[i].GetComponent<TransformComponent>().Location.y);
				data["objects"][i][loader.parser.objects[i].name][x]["transform"][0]["location"][2] = std::to_string(loader.parser.objects[i].GetComponent<TransformComponent>().Location.z);

				//Rotation
				data["objects"][i][loader.parser.objects[i].name][x]["transform"][1]["rotation"][0] = std::to_string(loader.parser.objects[i].GetComponent<TransformComponent>().Rotation.w);
				data["objects"][i][loader.parser.objects[i].name][x]["transform"][1]["rotation"][1] = std::to_string(loader.parser.objects[i].GetComponent<TransformComponent>().Rotation.x);
				data["objects"][i][loader.parser.objects[i].name][x]["transform"][1]["rotation"][2] = std::to_string(loader.parser.objects[i].GetComponent<TransformComponent>().Rotation.y);
				data["objects"][i][loader.parser.objects[i].name][x]["transform"][1]["rotation"][3] = std::to_string(loader.parser.objects[i].GetComponent<TransformComponent>().Rotation.z);

				//Scale
				data["objects"][i][loader.parser.objects[i].name][x]["transform"][2]["scale"][0] = std::to_string(loader.parser.objects[i].GetComponent<TransformComponent>().Scale.x);
				data["objects"][i][loader.parser.objects[i].name][x]["transform"][2]["scale"][1] = std::to_string(loader.parser.objects[i].GetComponent<TransformComponent>().Scale.y);
				data["objects"][i][loader.parser.objects[i].name][x]["transform"][2]["scale"][2] = std::to_string(loader.parser.objects[i].GetComponent<TransformComponent>().Scale.z);
			}
			else if (std::holds_alternative<ModelComponent>(loader.parser.objects[i].components[x])) {
				data["objects"][i][loader.parser.objects[i].name][x]["model"][0]["name"] = loader.parser.objects[i].GetComponent<ModelComponent>().model_name;
				data["objects"][i][loader.parser.objects[i].name][x]["model"][1]["path"] = loader.parser.objects[i].GetComponent<ModelComponent>().model_path;
				data["objects"][i][loader.parser.objects[i].name][x]["model"][2]["shader"] = loader.parser.objects[i].GetComponent<ModelComponent>().shader;
				data["objects"][i][loader.parser.objects[i].name][x]["model"][3]["instancing"] = std::to_string(loader.parser.objects[i].GetComponent<ModelComponent>().instancing);
			}
			else if (std::holds_alternative<LightComponent>(loader.parser.objects[i].components[x])) {
				
				switch (loader.parser.objects[i].GetComponent<LightComponent>().type){
					case LT_DIRECTIONNAL:
						//Type
						data["objects"][i][loader.parser.objects[i].name][x]["light"][0]["type"] = "directionnal";

						//Direction
						data["objects"][i][loader.parser.objects[i].name][x]["light"][1]["direction"][0] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().direction.x);
						data["objects"][i][loader.parser.objects[i].name][x]["light"][1]["direction"][1] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().direction.y);
						data["objects"][i][loader.parser.objects[i].name][x]["light"][1]["direction"][2] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().direction.z);

						//Ambient
						data["objects"][i][loader.parser.objects[i].name][x]["light"][2]["ambient"][0] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().ambient.x);
						data["objects"][i][loader.parser.objects[i].name][x]["light"][2]["ambient"][1] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().ambient.y);
						data["objects"][i][loader.parser.objects[i].name][x]["light"][2]["ambient"][2] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().ambient.z);
					
						//Diffuse
						data["objects"][i][loader.parser.objects[i].name][x]["light"][3]["diffuse"][0] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().diffuse.x);
						data["objects"][i][loader.parser.objects[i].name][x]["light"][3]["diffuse"][1] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().diffuse.y);
						data["objects"][i][loader.parser.objects[i].name][x]["light"][3]["diffuse"][2] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().diffuse.z);

						//Specular
						data["objects"][i][loader.parser.objects[i].name][x]["light"][4]["specular"][0] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().specular.x);
						data["objects"][i][loader.parser.objects[i].name][x]["light"][4]["specular"][1] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().specular.y);
						data["objects"][i][loader.parser.objects[i].name][x]["light"][4]["specular"][2] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().specular.z);
					
						//Intensity
						data["objects"][i][loader.parser.objects[i].name][x]["light"][5]["intensity"] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().intensity);
				
						//Color
						data["objects"][i][loader.parser.objects[i].name][x]["light"][6]["color"][0] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().color.x);
						data["objects"][i][loader.parser.objects[i].name][x]["light"][6]["color"][1] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().color.y);
						data["objects"][i][loader.parser.objects[i].name][x]["light"][6]["color"][2] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().color.z);
						
						break;
					
					case LT_POINT:
						//Type
						data["objects"][i][loader.parser.objects[i].name][x]["light"][0]["type"] = "point";

						//Ambient
						data["objects"][i][loader.parser.objects[i].name][x]["light"][1]["ambient"][0] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().ambient.x);
						data["objects"][i][loader.parser.objects[i].name][x]["light"][1]["ambient"][1] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().ambient.y);
						data["objects"][i][loader.parser.objects[i].name][x]["light"][1]["ambient"][2] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().ambient.z);

						//Diffuse
						data["objects"][i][loader.parser.objects[i].name][x]["light"][2]["diffuse"][0] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().diffuse.x);
						data["objects"][i][loader.parser.objects[i].name][x]["light"][2]["diffuse"][1] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().diffuse.y);
						data["objects"][i][loader.parser.objects[i].name][x]["light"][2]["diffuse"][2] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().diffuse.z);

						//Specular
						data["objects"][i][loader.parser.objects[i].name][x]["light"][3]["specular"][0] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().specular.x);
						data["objects"][i][loader.parser.objects[i].name][x]["light"][3]["specular"][1] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().specular.y);
						data["objects"][i][loader.parser.objects[i].name][x]["light"][3]["specular"][2] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().specular.z);

						//Constant
						data["objects"][i][loader.parser.objects[i].name][x]["light"][4]["constant"] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().constant);
						
						//Linear
						data["objects"][i][loader.parser.objects[i].name][x]["light"][5]["linear"] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().linear);
						
						//Quadratic
						data["objects"][i][loader.parser.objects[i].name][x]["light"][6]["quadratic"] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().quadratic);
						
						//Intensity
						data["objects"][i][loader.parser.objects[i].name][x]["light"][7]["intensity"] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().intensity);

						//Color
						data["objects"][i][loader.parser.objects[i].name][x]["light"][8]["color"][0] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().color.x);
						data["objects"][i][loader.parser.objects[i].name][x]["light"][8]["color"][1] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().color.y);
						data["objects"][i][loader.parser.objects[i].name][x]["light"][8]["color"][2] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().color.z);
						
						break;
					
					case LT_SPOT:
						//Type
						data["objects"][i][loader.parser.objects[i].name][x]["light"][0]["type"] = "spot";

						//Direction
						data["objects"][i][loader.parser.objects[i].name][x]["light"][1]["direction"][0] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().direction.x);
						data["objects"][i][loader.parser.objects[i].name][x]["light"][1]["direction"][1] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().direction.y);
						data["objects"][i][loader.parser.objects[i].name][x]["light"][1]["direction"][2] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().direction.z);

						//Ambient
						data["objects"][i][loader.parser.objects[i].name][x]["light"][2]["ambient"][0] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().ambient.x);
						data["objects"][i][loader.parser.objects[i].name][x]["light"][2]["ambient"][1] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().ambient.y);
						data["objects"][i][loader.parser.objects[i].name][x]["light"][2]["ambient"][2] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().ambient.z);

						//Diffuse
						data["objects"][i][loader.parser.objects[i].name][x]["light"][3]["diffuse"][0] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().diffuse.x);
						data["objects"][i][loader.parser.objects[i].name][x]["light"][3]["diffuse"][1] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().diffuse.y);
						data["objects"][i][loader.parser.objects[i].name][x]["light"][3]["diffuse"][2] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().diffuse.z);

						//Specular
						data["objects"][i][loader.parser.objects[i].name][x]["light"][4]["specular"][0] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().specular.x);
						data["objects"][i][loader.parser.objects[i].name][x]["light"][4]["specular"][1] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().specular.y);
						data["objects"][i][loader.parser.objects[i].name][x]["light"][4]["specular"][2] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().specular.z);

						//Constant
						data["objects"][i][loader.parser.objects[i].name][x]["light"][5]["constant"] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().constant);

						//Linear
						data["objects"][i][loader.parser.objects[i].name][x]["light"][6]["linear"] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().linear);

						//Quadratic
						data["objects"][i][loader.parser.objects[i].name][x]["light"][7]["quadratic"] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().quadratic);
						
						//CutOff
						data["objects"][i][loader.parser.objects[i].name][x]["light"][8]["cutOff"] = std::to_string(glm::degrees(glm::acos(loader.parser.objects[i].GetComponent<LightComponent>().cutOff)));

						//OuterCutOff
						data["objects"][i][loader.parser.objects[i].name][x]["light"][9]["outerCutOff"] = std::to_string(glm::degrees(glm::acos(loader.parser.objects[i].GetComponent<LightComponent>().outerCutOff)));

						//Intensity
						data["objects"][i][loader.parser.objects[i].name][x]["light"][10]["intensity"] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().intensity);

						//Color
						data["objects"][i][loader.parser.objects[i].name][x]["light"][11]["color"][0] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().color.x);
						data["objects"][i][loader.parser.objects[i].name][x]["light"][11]["color"][1] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().color.y);
						data["objects"][i][loader.parser.objects[i].name][x]["light"][11]["color"][2] = std::to_string(loader.parser.objects[i].GetComponent<LightComponent>().color.z);

						break;
				}


			}
			
		}
	}
	

	// We write the data and export the file
	std::string jsonString = data.dump(4);

	std::ofstream outputFile;

	outputFile.open(filename);
	outputFile << jsonString;
	outputFile.close();
	

}
