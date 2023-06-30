#include "Vector3.h"
#pragma once
/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 final {
	float m[4][4];
};

// スケーリング行列
Matrix4x4 MakeScalematrix(const Vector3& scale);

// 回転行列
Matrix4x4 MakeRotationXMatrix(float theta);
Matrix4x4 MakeRotationYMatrix(float theta);
Matrix4x4 MakeRotationZMatrix(float theta);

// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& trans);

// アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& trans);

// 代入演算子オーバーロード
Matrix4x4& operator*=(Matrix4x4& m1, const Matrix4x4& m2);

// 2公演算子オーバーロード
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m);

/// <summary>
///  ビューポート変換行列 (スクリーン座標にするための奴)
/// </summary>
/// <param name="left">ビューポートオフセットX</param>
/// <param name="top">ビューポートオフセットY</param>
/// <param name="width">ビューポート横幅</param>
/// <param name="height">ビューポート縦幅</param>
/// <returns></returns>
Matrix4x4 MakeVieportMatrix(float left, float top, float width, float height);



Matrix4x4 Matrix4x4Add(const Matrix4x4& m1, const Matrix4x4& m2);