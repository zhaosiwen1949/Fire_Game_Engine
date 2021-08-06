#include "CubicBezier.h"

namespace Alice{
	void CubicBezier::Init(float x1, float y1, float x2, float y2){
		mPoint1.Set(x1, y1);
		mPoint2.Set(x2, y2);

		maX = 3 * x1 - 3 * x2 + 1;
		mbX = 3 * x2 - 6 * x1;
		mcX	= 3 * x1;

		maY = 3 * y1 - 3 * y2 + 1;
		mbY = 3 * y2 - 6 * y1;
		mcY = 3 * y1;
	}
	float CubicBezier::GetXValueFromPercent(float percent){
		return ((maX * percent + mbX) * percent + mcX) * percent;
	}
	float CubicBezier::GetYValueFromPercent(float percent){
		return ((maY * percent + mbY) * percent + mcY) * percent;
	}
}