#include "Dynamics.h"


void PhysicsWorld::AddObject(PhysicsObject* objet)
{
	m_objects.push_back(objet);
}


void PhysicsWorld::RemoveObject(PhysicsObject* object) {
	if (!object) return;
	auto itr = std::find(m_objects.begin(), m_objects.end(), object);
	if (itr == m_objects.end()) return;
	m_objects.erase(itr);
}

void PhysicsWorld::RemoveAllObjects() {
	m_objects.clear();
}

void PhysicsWorld::Step(float dt) {

	ResolveCollisions(dt);

	for (PhysicsObject* obj : m_objects) {
		obj->force += obj->mass * m_gravity;
		

		obj->velocity += obj->force / obj->mass * dt;

		obj->transform->Location += obj->velocity * dt;
		obj->force = glm::vec3(0, 0, 0);
	}
}

void PhysicsWorld::ResolveCollisions(float dt) {
	std::vector<Collision> collisions;
	for (PhysicsObject* a : m_objects) {
		for (PhysicsObject* b : m_objects) {
			if (a == b) break;
			if (!a->collider || !b->collider)
				continue;

			CollisionPoints points = collision_test::TestCollision(a->collider, a->transform, b->collider, b->transform);

			if (points.HasCollision) {
				collisions.emplace_back(a, b, points);
			}
		}
	}
}
