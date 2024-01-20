#include "Collision.h"

physics::CollisionPoints::CollisionPoints() : A(glm::vec3(0.0f))
, B(glm::vec3(0.0f))
, Normal(glm::vec3(0.0f))
, Depth(0.0f)
, HasCollision(false)
{}

physics::CollisionPoints::CollisionPoints(glm::vec2 a, glm::vec2 b)
	: A(a, 0.f), B(b, 0.f), HasCollision(true)
{
	glm::vec2 ba = a - b;
	Depth = glm::length(ba);
	if (Depth > 0.00001f)
	{
		Normal = glm::vec3(ba / Depth, 0.f);
	}
	else {
		Normal = glm::vec3(0, 1, 0);
		Depth = 1;
	}
}

physics::CollisionPoints::CollisionPoints(glm::vec3 a, glm::vec3 b)
	: A(a)
	, B(b)
	, HasCollision(true)
{
	glm::vec3 ba = a - b;
	Depth = glm::length(ba);
	if (Depth > 0.00001f)
	{
		Normal = ba / Depth;
	}
	else {
		Normal = glm::vec3(0, 1, 0);
		Depth = 1;
	}
}

physics::CollisionPoints::CollisionPoints(glm::vec3 a, glm::vec3 b, glm::vec3 n, float d)
	: A(a)
	, B(b)
	, Normal(n)
	, Depth(d)
	, HasCollision(true)
{}

physics::CollisionPoints::CollisionPoints(glm::vec2 a, glm::vec2 b, glm::vec2 n, float d)
	: A(a, 0)
	, B(b, 0)
	, Normal(n, 0.0f)
	, Depth(d)
	, HasCollision(true)
{}

physics::CollisionPoints physics::collision_test::FindSphereSphereCollisionPoints(const physics::Collider* a, const Transform* ta, const physics::Collider* b, const Transform* tb)
{
	assert(a->Type == ColliderType::SPHERE && b->Type == ColliderType::SPHERE);

	Sphere* A = (Sphere*)a;
	Sphere* B = (Sphere*)b;

	vec_3 aCenter = A->center + (vec_3)ta->Location;
	vec_3 bCenter = B->center + (vec_3)tb->Location;

	vec_3 ab = bCenter - aCenter;

	scalar aRadius = A->radius * major(ta->Scale);
	scalar bRadius = B->radius * major(tb->Scale);

	scalar distance = length(ab);

	if (distance < 0.00001f
		|| distance > aRadius + bRadius)
	{
		return physics::CollisionPoints();
	}

	vec_3 normal = normalize(ab);

	vec_3 aDeep = aCenter + normal * aRadius;
	vec_3 bDeep = bCenter - normal * bRadius;

	return physics::CollisionPoints(aDeep, bDeep);
}

physics::CollisionPoints physics::collision_test::FindPlaneSphereCollisionPoints(const physics::Collider* a, const Transform* ta, const physics::Collider* b, const Transform* tb)
{
	assert(a->Type == ColliderType::PLANE && b->Type == ColliderType::SPHERE);

	Plane* A = (Plane*)a;
	Sphere* B = (Sphere*)b;

	vec_3  aCenter = B->center + (vec_3)tb->Location;
	scalar aRadius = B->radius * major(tb->Scale);

	vec_3 normal = rot_vec(glm::normalize(A->normal), ta);
	vec_3 onPlane = normal * A->distance + (vec_3)ta->Location;

	scalar distance = dot(aCenter - onPlane, normal); // distance from center of sphere to plane surface

	if (distance > aRadius) {
		return physics::CollisionPoints();
	}

	vec_3 aDeep = aCenter - normal * aRadius;
	vec_3 bDeep = aCenter - normal * distance;

	return physics::CollisionPoints(aDeep, bDeep, normal, distance);
}

physics::Collider::Collider(ColliderType type) : Type(type)
{
}

physics::SphereCollider::SphereCollider(glm::vec3 _center, float _radius)
	: Collider(ColliderType::SPHERE), center(_center), radius(_radius) {
}

physics::PlaneCollider::PlaneCollider(glm::vec3 _normal, float _distance)
	: Collider(ColliderType::PLANE), normal(_normal), distance(_distance) {
}