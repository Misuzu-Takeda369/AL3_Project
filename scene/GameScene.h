#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Enemy.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// モデル
	Model* model_ = nullptr;

	// ビュープロジェクション(ゲーム共通カメラ)
	ViewProjection viewProjection_;
	/// <summary>
	/// ゲームシーン用
	/// </summary>

	// 自分のキャラのデータ
	Player* player_ = nullptr;
	//敵のデータ
	Enemy* enemy_ = nullptr;

	// きゃめら
	// デバッグカメラの挙動
	bool isDebugCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;
};