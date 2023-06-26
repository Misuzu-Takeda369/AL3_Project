#pragma once
#include "EnemyBullet.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <list>

// 前方宣言
class Player;
class GameScene;

class Enemy {

public:
	~Enemy();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, Vector3 pos);

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

	/// <summary>
	/// ワールド行列に変換する関数
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

	// 当たったことが伝わったらこっちで処理する関数
	void OnCollision();

	// 弾リストのげったー
	// const std::list<EnemyBullet*>& GetBullet() { return bullets_; };

	// 敵が作った弾をゲームシーンに覚えさせておく奴
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; };

	/// <summary>
	/// クラスの外部用敵が消えるためのフラグ変数
	/// </summary>
	bool IsDead() const { return isDead_; };

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

	// フェーズ(初期化もして置く)
	Phase phase_ = Enemy::Phase::Approach;

	// 弾(プレイヤーと同じようにリスト化しとく)
	// std::list<EnemyBullet*> bullets_;

	// 発射タイマー
	int32_t fireTimer_ = 0;

	Player* player_ = nullptr;

	// ゲームシーン
	GameScene* gameScene_ = nullptr;

	// 弾の寿命(消えるまでの時間の定数)
	static const int32_t kLifeTime = 60 * 5;
	// 弾の寿命(消えるまでの時間の判定用変数)
	int32_t deathTimer_ = kLifeTime;
	// 消えるフラグ
	bool isDead_ = false;
};