#include "Bezier.h"

namespace Alice
{
	Bezier::Bezier()
	{

	}

	Bezier::Bezier(Bezier&other)
	{
		memcpy(this,&other,sizeof(Bezier));
	}

	void Bezier::Init(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4)
	{
		mPoints[0].Set(x1, y1, z1);
		mPoints[1].Set(x2, y2, z2);
		mPoints[2].Set(x3, y3, z3);
		mPoints[3].Set(x4, y4, z4);
		ma = 3 * x2 - x1 - 3 * x3 + x4;
		mb = 3 * (x1 - 2 * x2 + x3);
		mc = 3 * (x2-x1);

	}

	Alice::Vector3f Bezier::GetPointOnBezier(float x)
	{
		Alice::Vector3f ret;
		ret.x = x;
		float t = (x - mPoints[0].x) / (mPoints[3].x - mPoints[0].x);
		ret.x = mPoints[0].x*(1 - t)*(1 - t)*(1 - t) + 3 * mPoints[1].x*t*(1 - t)*(1 - t) + 3 * mPoints[2].x*t*t*(1 - t) + mPoints[3].x*t*t*t;
		ret.y = mPoints[0].y*(1 - t)*(1 - t)*(1 - t) + 3 * mPoints[1].y*t*(1 - t)*(1 - t) + 3 * mPoints[2].y*t*t*(1 - t) + mPoints[3].y*t*t*t;
		ret.z = mPoints[0].z*(1 - t)*(1 - t)*(1 - t) + 3 * mPoints[1].z*t*(1 - t)*(1 - t) + 3 * mPoints[2].z*t*t*(1 - t) + mPoints[3].z*t*t*t;
		return ret;
	}

	void Bezier::InitDValueWithX(float x)
	{
		md = mPoints[0].x - x;
	}

	void Bezier::InitDValueWithY(float y)
	{
		md = mPoints[0].y - y;
	}

	float Bezier::GetTValue()
	{
		float tValue = 0.0f;
		//Debug("%f %f %f %f",ma,mb,mc,md);
		mA = mb*mb - 3 * ma*mc;
		mB = mb*mc - 9 * ma*md;
		mC = mc*mc - 3 * mb*md;
		float delta = mB*mB - 4 * mA*mC;

		//Debug("A,B,C %f %f %f delta %f", mA, mB, mC,delta);
		if (0==mA&&0==mB)
		{
			tValue = -mc / mb;
		}
		else if (delta>0)
		{
			float Y1 = mA*mb + 3 * ma*((-mB + sqrtf(mB*mB-4*mA*mC))/2.0f);
			float Y2 = mA*mb + 3 * ma*((-mB - sqrtf(mB*mB - 4 * mA*mC)) / 2.0f);
			tValue = (-mb-(pow(Y1,1.0f/3.0f)+pow(Y2,1.0f/3.0f))) / (3 * ma);
		}
		else if (delta==0)
		{
			float k = mB / mA;
			float root = -mb / ma + k;
			if (root > 0.0f&&root <= 1.0f)
			{
				return root;
			}
			root = -k / 2.0f;
			if (root > 0.0f&&root <= 1.0f)
			{
				return root;
			}
		}
		else if (delta<0)
		{
			if (mA>0)
			{
				float t = (2 * mA*mb - 3 * ma*mB) / (2 * sqrt(mA*mA*mA));
				if (t>-1.0f&&t<1.0f)
				{
					float th = acosf(t);
					float root1 = (-mb - 2 * sqrtf(mA)*cosf(th/3.0f))/(3*ma);
					float root2 = (-mb + sqrtf(mA)*(cosf(th / 3.0f) + sqrtf(3.0f)*sinf(th / 3.0f))) / (3 * ma);
					float root3 = (-mb + sqrtf(mA)*(cosf(th / 3.0f) - sqrtf(3.0f)*sinf(th / 3.0f))) / (3 * ma);
					//Debug("roots %f %f %f",root1,root2,root3);
					if (root1>=0.0f&&root1<=1.0f)
					{
						tValue = root1;
					}
					else if (root2>=0.0f&&root2<=1.0f)
					{
						tValue = root2;
					}
					else if (root3 >= 0.0f&&root3 <= 1.0f)
					{
						tValue = root3;
					}
				}
			}
		}
		//Debug("t value is %f",tValue);
		return tValue;
	}

	float Bezier::GetXValueWithY(float y)
	{
		InitDValueWithY(y);
		float t = GetTValue();
		float ret = mPoints[0].x*(1 - t)*(1 - t)*(1 - t) + 3 * mPoints[1].x*t*(1 - t)*(1 - t) + 3 * mPoints[2].x*t*t*(1 - t) + mPoints[3].x*t*t*t;
		return ret;
	}

	float Bezier::GetYValueWithX(float x)
	{
		InitDValueWithX(x);
		float t = GetTValue();
		float ret = mPoints[0].y*(1 - t)*(1 - t)*(1 - t) + 3 * mPoints[1].y*t*(1 - t)*(1 - t) + 3 * mPoints[2].y*t*t*(1 - t) + mPoints[3].y*t*t*t;
		return ret;
	}
}