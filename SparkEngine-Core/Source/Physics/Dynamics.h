#include <vector>
#include "../Level Management/Components.h"
#include "../Utils/Engine/EngineUtils.h"
#include "Collision.h"

using namespace physics;

class PhysicsWorld {
private:
	std::vector<PhysicsObject*> m_objects;
	glm::vec3 m_gravity = glm::vec3(0.0f,-9.81f,0.0f);

public:
	void AddObject(PhysicsObject* objet);

	void RemoveObject(PhysicsObject* object);

	void Step(float dt);

	void ResolveCollisions(float dt);

};