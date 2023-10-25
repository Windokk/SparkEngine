 #include<glm/gtc/type_ptr.hpp>
#include <string>

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
	std::string light_name;
	float intensity;
	glm::vec4 color;
};

#endif