#pragma once
#include "Input.h"
#include "Model.h"
#include "PlayerBullet.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Player {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewprojection);

	void Attack();

private:
	// ワールドトランスフォーム(プレイヤーに移動するカメラ個体ごと)
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textuerHandle_ = 0u;

	// キーボード入力
	Input* input_ = nullptr;

	PlayerBullet* bullet_ = nullptr;
};
