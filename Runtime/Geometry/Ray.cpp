#include "Runtime/RuntimePrefix.h"
#include "Runtime/Geometry/Ray.h"

namespace Alice
{
	float Ray::SqrtDistanceToPoint(const Vector3f& P) const
	{
		Vector3f v = mDirection;
		Vector3f w = P - mOrigin;

		float c1 = Dot(w, v);
		float c2 = Dot(v, v);
		float b = c1 / c2;

		Vector3f Pb = GetPoint(b);
		return SqrMagnitude(P - Pb);
	}
}