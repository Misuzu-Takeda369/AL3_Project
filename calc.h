#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"

// ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);