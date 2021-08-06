#include "Runtime/RuntimePrefix.h"
#include "Sphere.h"
#include "AABB.h"
#include "Runtime/Render/VertexData.h"
namespace Alice{
	void Sphere::Set(const Vector3f* inVertices, AliceUInt32 inHowmany){
		mRadius = 0.0f;
		mCenter = Vector3f(0, 0, 0);
		AliceUInt32 i;
		for (i = 0; i < inHowmany; i++)
			mRadius = std::max<float>(mRadius, SqrMagnitude(inVertices[i]));
		mRadius = sqrt(mRadius);
	}
	void Sphere::Set(VertexDataFull*vertex_data_full, AliceUInt32 count) {
		mRadius = 0.0f;
		mCenter = Vector3f(0, 0, 0);
		for (int i=0;i<count;++i){
			mRadius = std::max<float>(mRadius, vertex_data_full[i].mVertex.SqrtMagnitude());
		}
		mRadius = sqrtf(mRadius);
	}
	void Sphere::Set(const MinMaxAABB& aabb) {
		mCenter = Vector3f((aabb.mMax.x + aabb.mMin.x) / 2.0f, (aabb.mMax.y + aabb.mMin.y) / 2.0f, (aabb.mMax.z + aabb.mMin.z) / 2.0f);
		float deltaX = aabb.mMax.x - mCenter.x;
		float deltaY = aabb.mMax.y - mCenter.y;
		float deltaZ = aabb.mMax.z - mCenter.z;
		mRadius = sqrtf(ALICE_SQUARE(deltaX)+ALICE_SQUARE(deltaY)+ALICE_SQUARE(deltaZ));
	}
}
