#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// 当たり判定の基底クラス
/// </summary>

class Collider {

public:

	/// <summary>
	/// ゲッター
	/// </summary>
	/// <returns></returns>
	float GetRadius() { return radius_; };

	/// <summary>
	/// セッター
	/// </summary>
	/// <param name="r"></param>
	/// <returns></returns>
	float SetRadius(float& r) { 
		radius_ = r;
	};

	/// <summary>
	/// 当たり判定
	/// </summary>
	virtual void OnCollision();

	virtual Vector3 GetWorldPosition();


private:
	float radius_ = 1.0f;
};
