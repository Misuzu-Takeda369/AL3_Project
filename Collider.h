#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
//必要忘れるな!!
#include <cstdint>

/// <summary>
/// 当たり判定の基底クラス(当たり判定関連の奴)
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
	float SetRadius(float& r) { radius_ = r; };

	/// <summary>
	/// 当たり判定
	/// </summary>
	virtual void OnCollision();

	virtual Vector3 GetWorldPosition();

	/// <summary>
	/// 衝突属性(能動側)の取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetCollistionAttribute() { return collistionAttribute_; }

	/// <summary>
	/// 衝突属性(能動側)の設定
	/// </summary>
	/// <param name="a"></param>
	/// <returns></returns>
	void SetCollistionAttribute(uint32_t sttbute) { collistionAttribute_ = sttbute; }

	/// <summary>
	/// 衝突属性(受動側)の取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetCollistionMask() { return collistionMask_; }

	/// <summary>
	/// 衝突属性(受動側)の設定
	/// </summary>
	/// <param name="Mask"></param>
	/// <returns></returns>
	void SetCollistionMask(uint32_t Mask) { collistionMask_ = Mask; }


	private:
	float radius_ = 1.0f;

	// 衝突判定プレイヤー側
	uint32_t collistionAttribute_ = 0xffffffff;

	// 衝突判定エネミー側
	uint32_t collistionMask_ = 0xffffffff;
};
