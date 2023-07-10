#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include <cmath>

// ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

//正規化
Vector3 dir(const float& x, const float& y, const float& z);

/// <summary>
/// Linear Interpolation　(線形保管)
/// </summary>
/// <param name="x3">スタート位置</param>
/// <param name="v2">ゴール位置</param>
/// <param name="t">フレーム(プレイヤーがどのぐらい移動しているか)</param>
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2);

// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v);

// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2);

