/*

    SHUFFLE COMPANY SOURCE CODE

    sc_collision.h
    ------------------------------------
	Algorithms for Sphere-Ray and AABB-Ray collisions thanks to:
	www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection


*/

#ifndef SC_COLLISION
#define SC_COLLISION

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "glm/ext.hpp"

#include <cmath>

namespace sc
{
	class AABBCollider;
	class PointCollider;
	class RayCollider;

	class SphereCollider
	{
	public:
		glm::vec3 center;
		float radius;

		SphereCollider(glm::vec3 center, float radius);
		bool collide(SphereCollider* sc);
		bool collide(AABBCollider* aabb);
		bool collide(PointCollider* pc);
		bool collide(RayCollider* ray);
	};

	class AABBCollider
	{
	public:
		glm::vec3 min;
		glm::vec3 max;

		AABBCollider(glm::vec3 min, glm::vec3 max);
		bool collide(SphereCollider* sc);
		bool collide(AABBCollider* aabb);
		bool collide(PointCollider* pc);
		bool collide(RayCollider* ray);
	};

	class PointCollider
	{
	public:
		glm::vec3 point;

		PointCollider(glm::vec3 point);
		bool collide(SphereCollider* sc);
		bool collide(AABBCollider* aabb);
		bool collide(PointCollider* pc);
	};

	class RayCollider
	{
	public:
		glm::vec3 origin;
		glm::vec3 direction;

		RayCollider(glm::vec3 origin, glm::vec3 direction);
		bool collide(SphereCollider* sc);
		bool collide(AABBCollider* aabb);
	};
}

#endif