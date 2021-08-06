#pragma once
#include "Runtime/Math/Vector2.h"

namespace Alice
{
	class CubicBezier
	{
	public:
		Vector2f mPoint1, mPoint2;
		float maX, mbX, mcX, maY, mbY, mcY;
		void Init(float x1,float y1,float x2,float y2);
		float GetXValueFromPercent(float percent);
		float GetYValueFromPercent(float percent);
	};
}