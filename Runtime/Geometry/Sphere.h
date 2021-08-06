#pragma once
#include "Runtime/Math/Vector3.h"
#include <algorithm>
namespace Alice{
	class MinMaxAABB;
	struct VertexDataFull;
	class Sphere{
	public:
		float	mRadius;
		Vector3f	mCenter;
	public:
		Sphere() { mRadius = 0.0f; }
		Sphere(const Vector3f& p0, float r) { Set(p0, r); }
		void Set(const Vector3f& p0) { mCenter = p0;	mRadius = 0; }
		void Set(const Vector3f& p0, float r) { mCenter = p0;	mRadius = r; }
		void Set(const Vector3f& p0, const Vector3f& p1);
		void Set(const Vector3f* inVertices, AliceUInt32 inHowmany);
		void Set(VertexDataFull*vertex_data_full,AliceUInt32 count);
		void Set(const MinMaxAABB& aabb);
		Vector3f& GetCenter() { return mCenter; }
		const Vector3f& GetCenter()const { return mCenter; }
		float& GetRadius() { return mRadius; }
		const float& GetRadius()const { return mRadius; }
		bool IsInside(const Sphere& inSphere)const;
	};
	float CalculateSqrDistance(const Vector3f& p, const Sphere& s);
	bool Intersect(const Sphere& inSphere0, const Sphere& inSphere1);
	inline void Sphere::Set(const Vector3f& p0, const Vector3f& p1){
		Vector3f dhalf = (p1 - p0) * 0.5;
		mCenter = dhalf + p0;
		mRadius = Magnitude(dhalf);
	}
	inline bool Sphere::IsInside(const Sphere& inSphere)const{
		float sqrDist = SqrMagnitude(GetCenter() - inSphere.GetCenter());
		if (ALICE_SQUARE(GetRadius()) > sqrDist + ALICE_SQUARE(inSphere.GetRadius()))
			return true;
		else
			return false;
	}
	inline bool Intersect(const Sphere& inSphere0, const Sphere& inSphere1){
		float sqrDist = SqrMagnitude(inSphere0.GetCenter() - inSphere1.GetCenter());
		if (ALICE_SQUARE(inSphere0.GetRadius() + inSphere1.GetRadius()) > sqrDist)
			return true;
		else
			return false;
	}
	inline float CalculateSqrDistance(const Vector3f& p, const Sphere& s){
		return ALICE_MAX(0.0F, SqrMagnitude(p - s.GetCenter()) - ALICE_SQUARE(s.GetRadius()));
	}
}
