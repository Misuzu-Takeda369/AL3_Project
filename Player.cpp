#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle) {
	// ヌルじゃないか確認
	assert(model);
	// 外から貰ってきたデータの受け渡し
	model_ = model;
	textuerHandle_ = textureHandle;

	//  ワールドトランスフォーム初期化(プレイヤーに移動するカメラ個体ごと)
	worldTransform_.Initialize();

	// 外から受け取ったのdelete不可能
};

void Player::Update() { worldTransform_.TransferMatrix(); };

void Player::Draw(ViewProjection viewprojection) {

	model_->Draw(worldTransform_, viewprojection, textuerHandle_);
};