#pragma once

#ifndef SceneParser_CLASS_H
#define SceneParser_CLASS_H

#include"../Basic Rendering/Shader/Shader.h"
#include"../Utils/Engine/EngineUtils.h"
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
	std::vector<std::variant<TransformComponent,
                                 ModelComponent,
                                 LightComponent, 
                        SphereColliderComponent, 
                         PlaneColliderComponent, 
                             RigidbodyComponent>> components;

    template <typename T>
    bool HasComponent() {
        for (const auto& variant : components) {
            if (std::holds_alternative<T>(variant)) {
                return true;
            }
        }
        return false;
    }

    template <typename T>
    T& GetComponent() {
        return *std::get_if<T>(&components[0]);
    }

    template <>
    TransformComponent& GetComponent<TransformComponent>() {
        for (auto& component : components) {
            if (auto pComponent = std::get_if<TransformComponent>(&component)) {
                return *pComponent;
            }
        }
        throw std::runtime_error("TransformComponent not found");
    }

    template <>
    ModelComponent& GetComponent<ModelComponent>() {
        for (auto& component : components) {
            if (auto pComponent = std::get_if<ModelComponent>(&component)) {
                return *pComponent;
            }
        }
        throw std::runtime_error("ModelComponent not found");
    }

    template <>
    LightComponent& GetComponent<LightComponent>() {
        for (auto& component : components) {
            if (auto pComponent = std::get_if<LightComponent>(&component)) {
                return *pComponent;
            }
        }
        throw std::runtime_error("LightComponent not found");
    }

    template <>
    SphereColliderComponent& GetComponent<SphereColliderComponent>() {
        for (auto& component : components) {
            if (auto pComponent = std::get_if<SphereColliderComponent>(&component)) {
                return *pComponent;
            }
        }
        throw std::runtime_error("SphereColliderComponent not found");
    }

    template <>
    PlaneColliderComponent& GetComponent<PlaneColliderComponent>() {
        for (auto& component : components) {
            if (auto pComponent = std::get_if<PlaneColliderComponent>(&component)) {
                return *pComponent;
            }
        }
        throw std::runtime_error("PlaneColliderComponent not found");
    }

    template <>
    RigidbodyComponent& GetComponent<RigidbodyComponent>() {
        for (auto& component : components) {
            if (auto pComponent = std::get_if<RigidbodyComponent>(&component)) {
                return *pComponent;
            }
        }
        throw std::runtime_error("RigidbodyComponent not found");
    }
};

class LevelParser
{
public:
	LevelParser(const char* file);

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