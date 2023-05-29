#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class EnemyBullet 
{
public:
	/// <summary>
	///
	/// </summary>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	void Update();

	void Draw(ViewProjection& view);

	/// <summary>
	/// クラスの外部用弾消えるためのフラグ変数
	/// </summary>
	bool IsDead() const { return isDead_; };

	// 当たったことが伝わったらこっちで処理する関数
	void OnCollision();

	Vector3 GetWorldPosition();

private:
	WorldTransform world_;
	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// 速度
	Vector3 velocity_;

	// 弾の寿命(消えるまでの時間の定数)
	static const int32_t kLifeTime = 60 * 5;
	// 弾の寿命(消えるまでの時間の判定用変数)
	int32_t deathTimer_ = kLifeTime;
	// 消えるフラグ
	bool isDead_ = false;
};
