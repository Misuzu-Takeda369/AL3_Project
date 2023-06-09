#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Enemy {

public:
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
	
	/// <summary>
	/// メンバ関数のポインターのテーブル
	/// イメージはEnemyクラスにある関数を配列(ポインタ)で扱う
	/// </summary>
	static void (Enemy::*spFuncTable[])();

	size_t phaseNow = static_cast<size_t>(phase_);
};
