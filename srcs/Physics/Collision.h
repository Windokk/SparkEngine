#include <glm/ext/vector_float3.hpp>
#include "../Level Management/LevelLoader.h"

struct CollisionPoints {
	glm::vec3 A;
	glm::vec3 B;
	glm::vec3 Normal;
	float depth;
	bool hasCollision;

};

struct Collider {
	virtual CollisionPoints TestCollision(
		const Transform* transform,
		const Collider* collider,
		const Transform* colliderTransform) const = 0;

	virtual CollisionPoints TestCollision(
		const Transform* transform,
		const SphereCollider* sphere,
		const Transform* sphereTransform) const = 0;

	virtual CollisionPoints TestCollision(
		const Transform* transform,
		const PlaneCollider* plane,
		const Transform* planeTransform) const = 0;
};

struct Collision {
	Object* Obj1;
	Object* ObjB;
	CollisionPoints Points;
};

struct SphereCollider : Collider {

	glm::vec3 center;
	float radius;

	virtual CollisionPoints TestCollision(
		const Transform* transform,
		const Collider* collider,
		const Transform* colliderTransform) const override
	{
		return collider->TestCollision(colliderTransform, this, transform);
	}

	virtual CollisionPoints TestCollision(
		const Transform* transform,
		const SphereCollider* sphere,
		const Transform* sphereTransform) const override
	{
		return algo::FindSphereSphereCollisionPoints(
			this, transform, sphere, sphereTransform);
	}

	virtual CollisionPoints TestCollision(
		const Transform* transform,
		const PlaneCollider* plane,
		const Transform* planeTransform) const override
	{
		return algo::FindSpherePlaneCollisionPoints(
			this, transform, plane, planeTransform);
	}

};

struct PlaneCollider : Collider {

	glm::vec3 plane;
	float distance;

	virtual CollisionPoints TestCollision(
		const Transform* transform,
		const Collider* collider,
		const Transform* colliderTransform) const override
	{
		return collider->TestCollision(colliderTransform, this, transform);
	}

	virtual CollisionPoints TestCollision(
		const Transform* transform,
		const SphereCollider* sphere,
		const Transform* sphereTransform) const override
	{
		return algo::FindPlaneSphereCollisionPoints(
			this, transform, sphere, sphereTransform);
	}

	virtual CollisionPoints TestCollision(
		const Transform* transform,
		const PlaneCollider* plane,
		const Transform* planeTransform) const override
	{
		return {};
	}
};

namespace algo {
	CollisionPoints FindSphereSphereCollisionPoints(
		const SphereCollider* a, const Transform* ta,
		const SphereCollider* b, const Transform* tb);
	CollisionPoints FindSpherePlaneCollisionPoints(
		const SphereCollider* a, const Transform* ta,
		const PlaneCollider* b, const Transform* tb);
	CollisionPoints FindPlaneSphereCollisionPoints(
		const PlaneCollider* a, const Transform* ta,
		const SphereCollider* b, const Transform* tb);
}