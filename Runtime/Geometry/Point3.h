#pragma once
#include "Runtime/Math/Vector3.h"
namespace Alice {
	class Point3f {
	public:
		union{
			struct{
				float x, y, z;
			};
			float v[3];
		};
	public:
		Point3f() :x(0), y(0), z(0){}
		Point3f(float x, float y, float z):x(x),y(y),z(z){}
		Point3f operator+(const Vector3f&v) {
			return Point3f(x+v.v[0],y+v.v[1],z+v.v[2]);
		}
		void operator+=(const Vector3f&v) {
			x += v.v[0];
			y += v.v[1];
			z += v.v[2];
		}
		Vector3f operator-(const Point3f&v) {
			return Vector3f(x - v.v[0], y - v.v[1], z - v.v[2]);
		}
		Point3f operator-(const Vector3f&v) {
			return Point3f(x - v.v[0], y - v.v[1], z - v.v[2]);
		}

		void operator-=(const Vector3f&v) {
			x -= v.v[0];
			y -= v.v[1];
			z -= v.v[2];
		}
	};
}