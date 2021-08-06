#pragma once
#include "Runtime/Math/Vector3.h"
#include "Runtime/Math/AliceMatrix4x4.h"

namespace Alice
{
	class Bezier
	{
	protected:
		Vector3f mPoints[4];
		float ma, mb, mc, md;
		float mA, mB, mC;
	protected:
		void InitDValueWithX(float x);
		void InitDValueWithY(float y);
		float GetTValue();
	public:
		Bezier();
		Bezier(Bezier&other);
		void Init(float x1,float y1,float z1,float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4,float z4);
		Vector3f GetPointOnBezier(float x);
		float GetYValueWithX(float x);
		float GetXValueWithY(float y);
	};
}