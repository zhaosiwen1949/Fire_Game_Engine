#pragma once
#include <vector>
#include <map>
#include "Bezier.h"
#include "Runtime/Math/Vector3.h"

namespace Alice
{
	class Curve
	{
	protected:
		std::map<int, Vector3f*> mKeyPoints;
	public:
		void AddKeyPoint(int nIndex, Vector3f*keyPoint);
		void SetKeyPointData(int nIndex,float x,float y,float z);
		int PrevKeyPointFrameIndex(int nIndex);
		int NextKeyPointFrameIndex(int nIndex);
		int GetIndexOf(int nIndex);
		void ModityKeyPointKey(int original,int current);
		void DeleteFrame(int nIndex);
		std::map<int, Vector3f*>&GetKeyPoints();
		void ReleaseKeyPoints();
	};
}