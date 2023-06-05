#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// 天球クラス
/// </summary>
class Skydome {

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();


	/// <summary>
	/// 描写
	/// </summary>
	void Draw(ViewProjection viewprojection);


private:

	///ワールド変換データ
	WorldTransform worldTransform_;

	//モデルデータ
	Model* model_ = nullptr;

	///テクスチャハンドrう
	uint32_t textureHandle_ = 0u;
};
