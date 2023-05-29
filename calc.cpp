#include "calc.h"

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {

	Vector3 result = {
	    v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
	    v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
	    v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2],
	};

	return result;
}

Vector3 dir(const float& x, const float& y, const float& z) { 
	float length = sqrtf(x * x + y * y + z * z); 

	Vector3 dir = {x / length, y / length, z / length};

	return dir;
}