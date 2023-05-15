#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#pragma once

class PlayerBullet {
public:
	/// <summary>
	///
	/// </summary>
	void Initialize(Model* model, const Vector3& position);

	void Update();

	void Draw(ViewProjection& view);

private:
	WorldTransform world_;
	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
};
