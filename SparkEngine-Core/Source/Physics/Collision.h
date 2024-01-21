#ifndef COLLISION_H
#define COLLISION_H

#include <glm/ext/vector_float3.hpp>
#include <glm/vec2.hpp>
#include <Level Management/Components.h>

namespace physics {
	using vec_3 = glm::vec3;
	using scalar = float;

	struct CollisionPoints {
		glm::vec3 A;
		glm::vec3 B;
		glm::vec3 Normal;
		scalar Depth;
		bool HasCollision;

		CollisionPoints();
		CollisionPoints(glm::vec2 a, glm::vec2 b);
		CollisionPoints(glm::vec3 a, glm::vec3 b);
		CollisionPoints(glm::vec3 a, glm::vec3 b, glm::vec3 n, float d);
		CollisionPoints(glm::vec2 a, glm::vec2 b, glm::vec2 n, float d);

		void SwapPoints()
		{
			glm::vec3 t = A;
			A = B;
			B = t;
			Normal = -Normal;
		}
	};

	enum ColliderType {
		PLANE,
		SPHERE,
		CAPSULE,
		HULL,
		MESH
	};

	struct  Collider {
	public:
		ColliderType Type;
		Collider();
		Collider(ColliderType type);
	};

	struct SphereCollider : public Collider {
	public:
		glm::vec3 center;
		float radius;

		SphereCollider(glm::vec3 center, float radius);
	};

	struct PlaneCollider : public Collider {
	public:
		PlaneCollider(glm::vec3 normal, float distance);
		glm::vec3 normal;
		float distance;
	};

	using Sphere = SphereCollider;
	using Plane = PlaneCollider;

	namespace collision_test {
		using FindContactFunc = CollisionPoints(*)(const Collider*, const Transform*, const Collider*, const Transform*);

		CollisionPoints FindSphereSphereCollisionPoints(const Collider* a, const Transform* ta, const Collider* b, const Transform* tb);
		CollisionPoints FindPlaneSphereCollisionPoints(const Collider* a, const Transform* ta, const Collider* b, const Transform* tb);

		inline CollisionPoints TestCollision(const Collider* a, const Transform* at, const Collider* b, const Transform* bt) {
			static const FindContactFunc tests[2][2] =
			{
				// Sphere                          Plane
				{ FindSphereSphereCollisionPoints, nullptr }, // Sphere
				{ FindPlaneSphereCollisionPoints,  nullptr }  // Plane
			};

			bool swap = b->Type > a->Type;

			if (swap)
			{
				std::swap(a, b);
				std::swap(at, bt);
			}
			std::cout << "a->type :  "<< a->Type<<"    b->Type :   "<<b->Type<<"\n\n";
			CollisionPoints points = tests[(int)a->Type][(int)b->Type](a, at, b, bt);

			// if we swapped the order of the colliders, to keep the
			// results consistent, we need to swap the points
			if (swap)
			{
				std::swap(points.A, points.B);
				points.Normal = -points.Normal;
			}

			return points;
		}
	}

	inline float major(const glm::vec2& v) {
		float m = v.x;
		if (v.y > m) m = v.y;
		return m;
	}

	inline float major(const glm::vec3& v) {
		float m = v.x;
		if (v.y > m) m = v.y;
		if (v.z > m) m = v.z;
		return m;
	}

	inline vec_3 rot_vec(const vec_3 v, const Transform* t)
	{
		return v * t->Rotation;
	}

	struct PhysicsObject {
		TransformComponent* transform;
		Collider* collider;
		glm::vec3 velocity;
		glm::vec3 force;
		float mass;
	};

	struct Collision {
		PhysicsObject* Obj1;
		PhysicsObject* ObjB;
		CollisionPoints Points;
	};
}

#endif   // COLLISION_H