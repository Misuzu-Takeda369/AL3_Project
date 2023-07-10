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

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) 
{
	//tempは資料のP?
	Vector3 temp;

	temp = Add(Multiply((1 - t), v1), Multiply(t,v2));

	return temp;
}


// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;

	return result;
};

// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v) {
	Vector3 result;
	result.x = scalar * v.x;
	result.y = scalar * v.y;
	result.z = scalar * v.z;

	return result;
}

// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;

	return result;
};