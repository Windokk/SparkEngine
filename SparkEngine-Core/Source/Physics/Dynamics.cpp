#include "Dynamics.h"

void physics::PhysicsWorld::AddObject(physics::PhysicsObject* objet)
{
	m_objects.push_back(objet);
}

void physics::PhysicsWorld::RemoveObject(physics::PhysicsObject* object) {
	if (!object) return;
	auto itr = std::find(m_objects.begin(), m_objects.end(), object);
	if (itr == m_objects.end()) return;
	m_objects.erase(itr);
}

void physics::PhysicsWorld::RemoveAllObjects() {
	m_objects.clear();
}

void physics::PhysicsWorld::Step(float dt) {
	//ResolveCollisions(dt);

	for (physics::PhysicsObject* obj : m_objects) {
		if (!obj->mass == 0) {
			obj->force += obj->mass * m_gravity;
			obj->velocity += obj->force / obj->mass * dt;
			obj->transform->Location += obj->velocity * dt;
			obj->force = glm::vec3(0, 0, 0);
		}
	}
}

void physics::PhysicsWorld::ResolveCollisions(float dt) {
	std::vector<physics::Collision> collisions;
	for (physics::PhysicsObject* a : m_objects) {
		for (physics::PhysicsObject* b : m_objects) {
			if (a == b) break;
			if (!a->collider || !b->collider)
				continue;

			physics::CollisionPoints points = collision_test::TestCollision(a->collider, &a->transform->toTransform(), b->collider, &b->transform->toTransform());

			if (points.HasCollision) {
				collisions.emplace_back(a, b, points);
			}
		}
	}
}