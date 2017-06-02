/*

    SHUFFLE COMPANY SOURCE CODE

    sc_collision.cpp
    ------------------------------------



*/

#include "sc_collision.h"

namespace sc
{
	SphereCollider::SphereCollider(glm::vec3 center, float radius)
	{
		this->center = center;
		this->radius = radius;
	}

	bool SphereCollider::collide(SphereCollider* sc)
	{
		float dx = sc->center.x - center.x;
		float dy = sc->center.y - center.y;
		float dz = sc->center.z - center.z;

		if ((dx * dx) + (dy * dy) + (dz * dz) <= (sc->radius + radius) * (sc->radius + radius))
		{
			return true;
		}

		return false;
	}

	bool SphereCollider::collide(AABBCollider* aabb)
	{
		float closestAABBX = fmax(aabb->min.x, fmin(center.x, aabb->max.x));
		float closestAABBY = fmax(aabb->min.y, fmin(center.y, aabb->max.y));
		float closestAABBZ = fmax(aabb->min.z, fmin(center.z, aabb->max.z));

		float dx = closestAABBX - center.x;
		float dy = closestAABBY - center.y;
		float dz = closestAABBZ - center.z;

		if ((dx * dx) + (dy * dy) + (dz * dz) <= radius * radius)
		{
			return true;
		}

		return false;
	}

	bool SphereCollider::collide(PointCollider* pc)
	{
		float dx = pc->point.x - center.x;
		float dy = pc->point.y - center.y;
		float dz = pc->point.z - center.z;

		if ((dx * dx) + (dy * dy) + (dz * dz) <= radius * radius)
		{
			return true;
		}

		return false;
	}

	bool SphereCollider::collide(RayCollider* ray)
	{
		//This assumes that direction of the ray is normalized, which it is on construction
		glm::vec3 l = ray->origin - center;
		float b = 2 * glm::dot(ray->direction, l);
		float c = glm::dot(l, l) - (radius * radius);

		float discriminant = (b * b) - (4 * c);

		if (discriminant > 0)
		{
			return true;
		}
		else if (discriminant == 0) // == with a float, for shame...
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool SphereCollider::collidePoint(RayCollider* ray, glm::vec3 &point)
	{
		//This assumes that direction of the ray is normalized, which it is on construction
		glm::vec3 l = ray->origin - center;
		float b = 2 * glm::dot(ray->direction, l);
		float c = glm::dot(l, l) - (radius * radius);

		float discriminant = (b * b) - (4 * c);

		if (discriminant > 0.0)
		{
			float t = (-b + (float)sqrt(discriminant)) / 2.0;
			point = ray->origin + (ray->direction * t);

			return true;
		}
		else if (discriminant == 0.0) // == with a float, for shame...
		{
			float t = -b / 2.0;
			point = ray->origin + (ray->direction * t);

			return true;
		}
		else
		{
			return false;
		}
	}



	AABBCollider::AABBCollider(glm::vec3 min, glm::vec3 max)
	{
		this->min = min;
		this->max = max;
	}

	bool AABBCollider::collide(SphereCollider* sc)
	{
		float closestAABBX = fmax(min.x, fmin(sc->center.x, max.x));
		float closestAABBY = fmax(min.y, fmin(sc->center.y, max.y));
		float closestAABBZ = fmax(min.z, fmin(sc->center.z, max.z));

		float dx = closestAABBX - sc->center.x;
		float dy = closestAABBY - sc->center.y;
		float dz = closestAABBZ - sc->center.z;

		if ((dx * dx) + (dy * dy) + (dz * dz) <= sc->radius * sc->radius)
		{
			return true;
		}

		return false;
	}

	bool AABBCollider::collide(AABBCollider* aabb)
	{
		if ((aabb->min.x <= max.x && aabb->max.x >= min.x) 
			&& (aabb->min.y <= max.y && aabb->max.y >= min.y) 
			&& (aabb->min.z <= max.z && aabb->max.z >= min.z))
		{
			return true;
		}

		return false;
	}

	bool AABBCollider::collide(PointCollider* pc)
	{
		if (pc->point.x >= min.x && pc->point.y >= min.y && pc->point.z >= min.z
			&& pc->point.x <= max.x && pc->point.y <= max.y && pc->point.z <= max.z)
		{
			return true;
		}

		return false;
	}

	bool AABBCollider::collide(RayCollider* ray)
	{
		glm::vec3 tMin;
		glm::vec3 tMax;

		tMin.x = (min.x - ray->origin.x) / ray->direction.x;
		tMax.x = (max.x - ray->origin.x) / ray->direction.x;

		if (tMin.x > tMax.x)
		{
			float temp = tMin.x;
			tMin.x = tMax.x;
			tMax.x = temp;
		}

		tMin.y = (min.y - ray->origin.y) / ray->direction.y;
		tMax.y = (max.y - ray->origin.y) / ray->direction.y;

		if (tMin.y > tMax.y)
		{
			float temp = tMin.y;
			tMin.y = tMax.y;
			tMax.y = temp;
		}

		if ((tMin.x > tMax.y) || (tMin.y > tMax.x))
		{
			return false;
		}

		if (tMin.y > tMin.x)
		{
			tMin.x = tMin.y;
		}

		if (tMax.y < tMax.x)
		{
			tMax.x = tMax.y;
		}

		tMin.z = (min.z - ray->origin.z) / ray->direction.z;
		tMax.z = (max.z - ray->origin.z) / ray->direction.z;

		if (tMin.z > tMax.z)
		{
			float temp = tMin.z;
			tMin.z = tMax.z;
			tMax.z = temp;
		}

		if ((tMin.x > tMax.z) || (tMin.z > tMax.x))
		{
			return false;
		}

		return true;
	}

	bool AABBCollider::collidePoint(RayCollider* ray, glm::vec3 &point)
	{
		glm::vec3 tMin;
		glm::vec3 tMax;

		tMin.x = (min.x - ray->origin.x) / ray->direction.x;
		tMax.x = (max.x - ray->origin.x) / ray->direction.x;

		if (tMin.x > tMax.x)
		{
			float temp = tMin.x;
			tMin.x = tMax.x;
			tMax.x = temp;
		}

		tMin.y = (min.y - ray->origin.y) / ray->direction.y;
		tMax.y = (max.y - ray->origin.y) / ray->direction.y;

		if (tMin.y > tMax.y)
		{
			float temp = tMin.y;
			tMin.y = tMax.y;
			tMax.y = temp;
		}

		if ((tMin.x > tMax.y) || (tMin.y > tMax.x))
		{
			return false;
		}

		if (tMin.y > tMin.x)
		{
			tMin.x = tMin.y;
		}

		if (tMax.y < tMax.x)
		{
			tMax.x = tMax.y;
		}

		tMin.z = (min.z - ray->origin.z) / ray->direction.z;
		tMax.z = (max.z - ray->origin.z) / ray->direction.z;

		if (tMin.z > tMax.z)
		{
			float temp = tMin.z;
			tMin.z = tMax.z;
			tMax.z = temp;
		}

		if ((tMin.x > tMax.z) || (tMin.z > tMax.x))
		{
			return false;
		}

		if (tMin.z > tMin.x)
		{
			tMin.x = tMin.z;
		}

		if (tMax.z < tMax.x)
		{
			tMax.x = tMax.z;		
		}

		point = tMin;

		return true;
	}



	PointCollider::PointCollider(glm::vec3 point)
	{
		this->point = point;
	}

	bool PointCollider::collide(SphereCollider* sc)
	{
		float dx = point.x - sc->center.x;
		float dy = point.y - sc->center.y;
		float dz = point.z - sc->center.z;

		if ((dx * dx) + (dy * dy) + (dz * dz) <= sc->radius * sc->radius)
		{
			return true;
		}

		return false;
	}

	bool PointCollider::collide(AABBCollider* aabb)
	{
		if (point.x >= aabb->min.x && point.y >= aabb->min.y && point.z >= aabb->min.z
			&& point.x <= aabb->max.x && point.y <= aabb->max.y && point.z <= aabb->max.z)
		{
			return true;
		}

		return false;
	}

	bool PointCollider::collide(PointCollider* pc)
	{
		if (pc->point.x == point.x && pc->point.y == point.y && pc->point.z == point.z)
		{
			return true;
		}

		return false;
	}



	RayCollider::RayCollider(glm::vec3 origin, glm::vec3 direction)
	{
		this->origin = origin;
		this->direction = glm::normalize(direction);
	}

	bool RayCollider::collide(SphereCollider* sc)
	{
		//This assumes that direction of the ray is normalized, which it is on construction
		glm::vec3 l = origin - sc->center;
		float b = 2 * glm::dot(direction, l);
		float c = glm::dot(l, l) - (sc->radius * sc->radius);

		float discriminant = (b * b) - (4 * c);

		if (discriminant > 0)
		{
			return true;
		}
		else if (discriminant == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool RayCollider::collide(AABBCollider* aabb)
	{
		glm::vec3 tMin;
		glm::vec3 tMax;

		tMin.x = (aabb->min.x - origin.x) / direction.x;
		tMax.x = (aabb->max.x - origin.x) / direction.x;

		if (tMin.x > tMax.x)
		{
			float temp = tMin.x;
			tMin.x = tMax.x;
			tMax.x = temp;
		}

		tMin.y = (aabb->min.y - origin.y) / direction.y;
		tMax.y = (aabb->max.y - origin.y) / direction.y;

		if (tMin.y > tMax.y)
		{
			float temp = tMin.y;
			tMin.y = tMax.y;
			tMax.y = temp;
		}

		if ((tMin.x > tMax.y) || (tMin.y > tMax.x))
		{
			return false;
		}

		if (tMin.y > tMin.x)
		{
			tMin.x = tMin.y;
		}

		if (tMax.y < tMax.x)
		{
			tMax.x = tMax.y;
		}

		tMin.z = (aabb->min.z - origin.z) / direction.z;
		tMax.z = (aabb->max.z - origin.z) / direction.z;

		if (tMin.z > tMax.z)
		{
			float temp = tMin.z;
			tMin.z = tMax.z;
			tMax.z = temp;
		}

		if ((tMin.x > tMax.z) || (tMin.z > tMax.x))
		{
			return false;
		}
		
		return true;
	}

	bool RayCollider::collidePoint(SphereCollider* sc, glm::vec3 &point)
	{
		//This assumes that direction of the ray is normalized, which it is on construction
		glm::vec3 l = origin - sc->center;
		float b = 2 * glm::dot(direction, l);
		float c = glm::dot(l, l) - (sc->radius * sc->radius);

		float discriminant = (b * b) - (4 * c);

		if (discriminant > 0.0)
		{
			float t = (-b + (float)sqrt(discriminant)) / 2.0;
			point = origin + (direction * t);

			return true;
		}
		else if (discriminant == 0.0) // == with a float, for shame...
		{
			float t = -b / 2.0;
			point = origin + (direction * t);

			return true;
		}
		else
		{
			return false;
		}
	}

	bool RayCollider::collidePoint(AABBCollider* aabb, glm::vec3 &point)
	{
		glm::vec3 tMin;
		glm::vec3 tMax;

		tMin.x = (aabb->min.x - origin.x) / direction.x;
		tMax.x = (aabb->max.x - origin.x) / direction.x;

		if (tMin.x > tMax.x)
		{
			float temp = tMin.x;
			tMin.x = tMax.x;
			tMax.x = temp;
		}

		tMin.y = (aabb->min.y - origin.y) / direction.y;
		tMax.y = (aabb->max.y - origin.y) / direction.y;

		if (tMin.y > tMax.y)
		{
			float temp = tMin.y;
			tMin.y = tMax.y;
			tMax.y = temp;
		}

		if ((tMin.x > tMax.y) || (tMin.y > tMax.x))
		{
			return false;
		}

		if (tMin.y > tMin.x)
		{
			tMin.x = tMin.y;
		}

		if (tMax.y < tMax.x)
		{
			tMax.x = tMax.y;
		}

		tMin.z = (aabb->min.z - origin.z) / direction.z;
		tMax.z = (aabb->max.z - origin.z) / direction.z;

		if (tMin.z > tMax.z)
		{
			float temp = tMin.z;
			tMin.z = tMax.z;
			tMax.z = temp;
		}

		if ((tMin.x > tMax.z) || (tMin.z > tMax.x))
		{
			return false;
		}

		if (tMin.z > tMin.x)
		{
			tMin.x = tMin.z;
		}

		if (tMax.z < tMax.x)
		{
			tMax.x = tMax.z;		
		}

		point = tMin;

		return true;
	}
}