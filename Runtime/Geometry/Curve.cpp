#include "Curve.h"
#include "Runtime/Allocator/DefaultAllocator.h"

namespace Alice
{
	void Curve::AddKeyPoint(int nIndex, Vector3f*keyPoint)
	{
		if (mKeyPoints.find(nIndex)== mKeyPoints.end())
		{
			mKeyPoints.insert(std::pair<int, Vector3f*>(nIndex, keyPoint));
		}
	}

	void Curve::ModityKeyPointKey(int original, int current)
	{
		std::map<int, Vector3f*>::iterator iter = mKeyPoints.find(original);
		if (iter != mKeyPoints.end())
		{
			Vector3f*point = iter->second;
			mKeyPoints.erase(iter);
			AddKeyPoint(current, point);
		}
	}

	int Curve::PrevKeyPointFrameIndex(int nIndex)
	{
		int prev = 0;
		std::map<int, Vector3f*>::iterator iter = mKeyPoints.begin();
		for (; iter != mKeyPoints.end(); iter++)
		{
			if (iter->first>=nIndex)
			{
				return prev;
			}
			prev = iter->first;
		}
		return prev;
	}

	int Curve::NextKeyPointFrameIndex(int nIndex)
	{
		std::map<int, Vector3f*>::iterator iter = mKeyPoints.begin();
		for (; iter != mKeyPoints.end(); iter++)
		{
			if (iter->first == nIndex)
			{
				iter++;
				if (iter != mKeyPoints.end())
				{
					return iter->first;
				}
				else
				{
					return -1;
				}
			}
		}
		return -1;
	}

	void Curve::DeleteFrame(int nIndex)
	{
		std::map<int, Vector3f*>::iterator iter = mKeyPoints.find(nIndex);
		if (iter!=mKeyPoints.end())
		{
			mKeyPoints.erase(iter);
		}
	}

	int Curve::GetIndexOf(int nIndex)
	{
		int nRet = 0;
		std::map<int, Vector3f*>::iterator iter = mKeyPoints.begin();
		for (; iter != mKeyPoints.end(); iter++)
		{
			if (iter->first == nIndex)
			{
				break;
			}
			nRet++;
		}
		return nRet;
	}

	void Curve::SetKeyPointData(int nIndex, float x, float y, float z)
	{
		std::map<int, Vector3f*>::iterator iter = mKeyPoints.find(nIndex);
		if (iter != mKeyPoints.end())
		{
			Vector3f*point = iter->second;
			point->Set(x,y,z);
		}
	}

	std::map<int, Vector3f*>&Curve::GetKeyPoints()
	{
		return mKeyPoints;
	}


	void Curve::ReleaseKeyPoints()
	{
		for (std::map<int, Vector3f*>::iterator iter = mKeyPoints.begin(); iter != mKeyPoints.end();iter++)
		{
			delete iter->second;
		}
		mKeyPoints.clear();
	}
}