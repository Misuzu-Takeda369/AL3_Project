#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include <cmath>

// ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

//正規化
Vector3 dir(const float& x, const float& y, const float& z);

//逆行列
Matrix4x4 Inverse(const Matrix4x4& m);