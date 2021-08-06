#include "Runtime/RuntimePrefix.h"
#include "Intersection.h"
#include "Ray.h"
#include "Plane.h"
#include "Sphere.h"
#include "AABB.h"
#include "Runtime/Debugger/Debugger.h"

#pragma warning(disable:4800)
namespace Alice
{
	bool IntersectRaySphere(Ray& ray,const Sphere& inSphere){
		Vector3f ray_to_sphere_center = inSphere.GetCenter() - ray.GetOrigin();
		float d = Dot(ray_to_sphere_center, ray.GetDirection());//判断是在ray的前面还是后面
		float lSqr = Dot(ray_to_sphere_center, ray_to_sphere_center);
		float rSqr = ALICE_SQUARE(inSphere.GetRadius());//半径的平方
		if (d < 0.0F && lSqr > rSqr)
			return false;
		float mSqr = lSqr - ALICE_SQUARE(d);
		if (mSqr > rSqr)
			return false;
		else
			return true;
	}
	bool IntersectSphereFrustumFull(const Vector3f& center, float radius, const Plane p[6])
	{
		for (int i = 0; i < 6; ++i, ++p)
		{
			float dist = p->GetDistanceToPoint(center);
			if (dist + radius < 0) return false;
		}
		return true;
	}

	bool IntersectAABBFrustumFull(AABB& a, Plane p[6])
	{
		const Vector3f& m = a.GetCenter();
		const Vector3f& extent = a.GetExtent();

		for (int i = 0; i < 6; ++i, ++p)
		{
			const Vector3f& normal = p->GetNormal();
			float dist = p->GetDistanceToPoint(m);
			float radius = Dot(extent, Abs(normal));
			if (dist + radius < 0) return false;
		}
		return true;
	}
}