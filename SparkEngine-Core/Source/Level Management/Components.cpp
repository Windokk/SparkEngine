#include "Components.h"

Transform::Transform(glm::vec3 location, glm::quat rotation, glm::vec3 scale)
	: Location(location), Rotation(rotation), Scale(scale) {
}

Component::Component()
{
}

TransformComponent::TransformComponent(glm::vec3& location, glm::quat& rotation, glm::vec3& scale)
	: Location(location), Rotation(rotation), Scale(scale) {
}

TransformComponent::TransformComponent() {
}

const Transform& TransformComponent::toTransform() const
{
	return Transform(TransformComponent::Location, TransformComponent::Rotation, TransformComponent::Scale);
}

TransformComponent Transform::toTransformComponent()
{
	return TransformComponent(Transform::Location, Transform::Rotation, Transform::Scale);
}

ModelComponent::ModelComponent(std::string _model_name, std::string _model_path, std::string _shader, int _instancing)
	: model_name(_model_name), model_path(_model_path), shader(_shader), instancing(_instancing) {
}
ModelComponent::ModelComponent() {
}

LightComponent::LightComponent(lightType _type, glm::vec3 _position, glm::vec3 _direction, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular, float _constant, float _linear, float _quadratic, float _cutOff, float _outerCutOff, float _intensity, glm::vec3 _color)
	: type(_type), position(_position), direction(_direction), ambient(_ambient), diffuse(_diffuse), specular(_specular), constant(_constant), linear(_linear), quadratic(_quadratic), cutOff(_cutOff), outerCutOff(_outerCutOff), intensity(_intensity), color(_color) {
}

LightComponent::LightComponent()
{
}

ColliderComponent::ColliderComponent()
{
}

PlaneColliderComponent::PlaneColliderComponent(glm::vec3 _normal, float _distance)
	: normal(_normal), distance(_distance), ColliderComponent() {
	Type = physics::PLANE;
}

PlaneColliderComponent::PlaneColliderComponent() : ColliderComponent() {
	Type = physics::PLANE;
}

SphereColliderComponent::SphereColliderComponent(glm::vec3 _center, float _radius)
	: center(_center), radius(_radius), ColliderComponent() {
	Type = physics::SPHERE;
}

SphereColliderComponent::SphereColliderComponent() : ColliderComponent() {
	Type = physics::SPHERE;
}


RigidbodyComponent::RigidbodyComponent(float _mass, ColliderComponent _collider)
	: mass(_mass), collider(_collider) {
}

RigidbodyComponent::RigidbodyComponent()
{
}

