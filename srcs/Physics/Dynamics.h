#include <vector>
#include <glm/ext/vector_float3.hpp>
#include "../Level Management/Components.h"
#include "../Utils/Engine/EngineUtils.h"
#include "../Level Management/LevelLoader.h"

struct Object {
	Transform* transform;
	glm::vec3 velocity;
	glm::vec3 force;
	float mass;
};

class PhysicsWorld {
private:
	std::vector<Object*> m_objects;
	glm::vec3 m_gravity = glm::vec3(0,-9.81f,0);

public:
	void AddObject(Object* objet) {
		m_objects.push_back(objet);
	}
	void RemoveObject(Object* object) {
		if (!object) return;
		auto itr = std::find(m_objects.begin(), m_objects.end(), object);
		if (itr == m_objects.end()) return;
		m_objects.erase(itr);
	}

	void Step(float dt) {
		for (Object* obj : m_objects) {
			obj->force += obj->mass * m_gravity;
			obj->velocity += obj->force / obj->mass * dt;
			obj->transform->Location += obj->velocity * dt;
			obj->force = glm::vec3(0, 0, 0);
		}
	}

};
