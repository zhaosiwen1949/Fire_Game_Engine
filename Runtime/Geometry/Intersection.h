#pragma once
#include "Runtime/Geometry/Ray.h"
#include "Runtime/Geometry/Sphere.h"
#include "Runtime/Geometry/AABB.h"
#include "Runtime/Geometry/Plane.h"
#include "Runtime/Math/Vector3.h"
namespace Alice{
	bool IntersectRaySphere(Ray& ray, const Sphere& inSphere);
	bool IntersectAABBFrustumFull(AABB& a, Plane p[6]);
	bool IntersectSphereFrustumFull(const Vector3f& center, float radius, const Plane p[6]);
}
