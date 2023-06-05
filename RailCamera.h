#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class RailCamera 
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描写
	/// </summary>
	void Draw();


private:

	/// ワールド変換データ
	WorldTransform worldTransform_;

	/// ビュープロジェクション
	ViewProjection viewProjection_;

	// モデル
	Model* model_ = nullptr;

};
