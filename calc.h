#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include <cmath>
#include <assert.h>

// ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

//正規化
Vector3 dir(const float& x, const float& y, const float& z);


/// 座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

/// <summary>
/// スカラー倍
/// </summary>
/// <param name="scalar">何倍か</param>
/// <param name="v">ベクトル</param>
/// <returns></returns>
Vector3 Multiply(float scalar, const Vector3& v);

/// <summary>
/// 減算
/// </summary>
/// <param name="v1">轢かれる方</param>
/// <param name="v2">引く法</param>
/// <returns></returns>
Vector3 Subtract(const Vector3& v1, const Vector3& v2);


/// <summary>
/// 足し算
/// </summary>
/// <param name="v1">足される</param>
/// <param name="v2">足す方</param>
Vector3 Add(const Vector3& v1, const Vector3& v2);
