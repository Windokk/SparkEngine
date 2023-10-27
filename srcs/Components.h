 #include<glm/gtc/type_ptr.hpp>
#include <string>

#include "EngineUtils.h"

#ifndef Component_CLASS_H
#define Component_CLASS_H

class Component {
public:
	Component();
private:

};

#endif

#ifndef TransformComponent_CLASS_H
#define TransformComponent_CLASS_H

class TransformComponent : public Component {
public :
	TransformComponent();
	glm::vec3 Location = glm::vec3(0, 0, 0);
	glm::quat Rotation = glm::quat(0, 0, 0, 0);
	glm::vec3 Scale = glm::vec3(1, 1, 1);
};

#endif

#ifndef ModelComponent_CLASS_H
#define ModelComponent_CLASS_H

class ModelComponent : public Component {
public:
	ModelComponent();
	std::string model_name;
	std::string model_path;
	std::string shader;
	int instancing;
};

#endif

#ifndef LightComponent_CLASS_H
#define LightComponent_CLASS_H

class LightComponent : public Component {
public:
	LightComponent();
	lightType type;
	glm::vec3 position;
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
};

#endif