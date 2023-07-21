#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <list>
#include "EnemyBullet.h"

//前方宣言
class Player;

class Enemy : public Collider{

public:

	~Enemy();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewprojection);

	/// <summary>
	/// 接近時の行動
	/// </summary>
	void ApproachMove();
	/// <summary>
	/// 離れる時の行動
	/// </summary>
	void LeaveMove();
	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();
	/// <summary>
	/// 接近フェーズ初期化
	/// </summary>
	void ApproachInt();

	/// <summary>
	/// 発射間隔(60フレーム毎に)
	/// </summary>
	static const int kFireInterval = 60;

	void SetPlayer(Player* player) { player_ = player; };

	void OnCollision() override;

	Vector3 GetWorldPosition() override;

	//弾リストのげったー
	const std::list<EnemyBullet*>& GetBullet() { return bullets_; };

private:
	// ワールドトランスフォーム(敵に移動するカメラ個体ごと)
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	enum class Phase {
		Approach, // 接近する
		Leave     // 離れる
	};

	//フェーズ(初期化もして置く)
	Phase phase_ = Enemy::Phase::Approach;

	//弾(プレイヤーと同じようにリスト化しとく)
	std::list<EnemyBullet*> bullets_;

	//発射タイマー
	int32_t fireTimer_ = 0;

	Player* player_ = nullptr;
};
