#include "Player.h"
#include "ImGuiManager.h"
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

	// シングルトンインスタンス取得
	input_ = Input::GetInstance();
};

void Player::Update() {

	Vector3 move = {};

	const float kCharactorspeed = 0.2f;

	// 入力処理
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharactorspeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharactorspeed;
	}
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharactorspeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharactorspeed;
	}
	// 制限
	// 制限座標
	const float kMoveLimiteX = 20.0f;
	const float kMoveLimiteY = 20.0f;
	// 範囲処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimiteX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimiteX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimiteY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimiteY);

	// 移動
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;
	// 転送
	worldTransform_.TransferMatrix();
	// 行列更新
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// ImGui
	// 窓
	ImGui::SetNextWindowPos({0, 0});
	ImGui::SetNextWindowSize({300, 100});
	// プレイヤー座標表示
	ImGui::Begin("Player");
	float sliderValue[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	ImGui::SliderFloat3("position", sliderValue, -20.0f, 20.0f);
	worldTransform_.translation_ = {sliderValue[0], sliderValue[1], sliderValue[2]};
	ImGui::End();
};

void Player::Draw(ViewProjection viewprojection) {

	model_->Draw(worldTransform_, viewprojection, textuerHandle_);
};