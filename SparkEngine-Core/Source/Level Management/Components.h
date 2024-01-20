#include<glm/gtc/type_ptr.hpp>
#include <string>
#include <../Utils/Engine/EngineUtils.h>

#ifndef TRANSFORM_H
#define TRANSFORM_H

class TransformComponent;

class Transform {
public:
	Transform(glm::vec3 location, glm::quat rotation, glm::vec3 scale);
	TransformComponent toTransformComponent();
	glm::vec3 Location;
	glm::quat Rotation;
	glm::vec3 Scale;
};

#endif // TRANSFORM_H

#ifndef Component_CLASS_H
#define Component_CLASS_H

class Component {
public:
	Component();
private:

};

#endif

#ifndef ModelComponent_CLASS_H
#define ModelComponent_CLASS_H

class ModelComponent : public Component {
public:
	ModelComponent(std::string _model_name, std::string _model_path, std::string _shader, int _instancing);
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
	LightComponent(lightType _type, glm::vec3 _position, glm::vec3 _direction, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular, float _constant, float _linear, float _quadratic, float _cutOff, float _outerCutOff, float _intensity, glm::vec3 _color);
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

#ifndef ColliderComponent_CLASS_H
#define ColliderComponent_CLASS_H

class ColliderComponent : public Component {
public:
	ColliderComponent();
};

#endif

#ifndef PlaneColliderComponent_CLASS_H
#define PlaneColliderComponent_CLASS_H

class PlaneColliderComponent : public ColliderComponent {
public:
	PlaneColliderComponent(glm::vec3 _normal, float _distance);
	PlaneColliderComponent();
	glm::vec3 normal;
	float distance;

};

#endif

#ifndef SphereColliderComponent_CLASS_H
#define SphereColliderComponent_CLASS_H

class SphereColliderComponent : public ColliderComponent {
public:
	SphereColliderComponent(glm::vec3 _center, float _radius);
	SphereColliderComponent();
	glm::vec3 center;
	float radius;
};

#endif

#ifndef RigidbodyComponent_CLASS_H
#define RigidbodyComponent_CLASS_H

class RigidbodyComponent : public Component {
public:
	RigidbodyComponent(float _mass, ColliderComponent _collider);
	RigidbodyComponent();
	float mass;
	ColliderComponent collider;

};

#endif


#ifndef TransformComponent_CLASS_H
#define TransformComponent_CLASS_H

class TransformComponent : public Component {
public:
	TransformComponent(glm::vec3& location, glm::quat& rotation, glm::vec3& scale);
	TransformComponent();
	const Transform& toTransform() const;
	glm::vec3 Location;
	glm::quat Rotation;
	glm::vec3 Scale;
};

#endif