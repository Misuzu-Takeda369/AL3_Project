#pragma once
#include "Input.h"
#include "Model.h"
#include "PlayerBullet.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <list>


class Player {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle,Vector3 pos);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewprojection);

	void Attack();

	/// <summary>
	/// ワールド行列に変換する関数
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

	//当たったことが伝わったらこっちで処理する関数
	void OnCollision();

	// 弾リストのげったー
	const std::list<PlayerBullet*>& GetBullet() { return bullets_; };

	/// <summary>
	/// 親となるワールドトランスフォーム(今回はカメラ)
	/// </summary>
	void SetParent(const WorldTransform * parent);

	/// <summary>
	/// 自機のワールド座標から3Dレティクルのワールド座標を計算関数
	/// </summary>
	void PtoReticleCalc();
	

private:
	// ワールドトランスフォーム(プレイヤーに移動するカメラ個体ごと)
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// キーボード入力
	Input* input_ = nullptr;

	//弾
	//PlayerBullet* bullet_ = nullptr;
	std::list<PlayerBullet*> bullets_;

	//3dレティクル(ターゲットの画像のようなもの)用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;

};
