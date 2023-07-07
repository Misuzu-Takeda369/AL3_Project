#include "EnemyBullet.h"
#include <cassert>
#include <cmath>
#include <iostream>

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {

	// 引数で受け取った速度をメンバ変数に入れる
	velocity_ = velocity;

	// ヌルポインター
	assert(model);
	model_ = model;

	textureHandle_ = TextureManager::Load("red1x1.png");

	// 初期化と初期位置を貰う
	world_.Initialize();
	world_.translation_ = position;
	// 敵の弾のスケールを変える
	world_.scale_ = {0.5f,0.5f,3.0f};

#pragma region プレイヤー方向を向こう!

	//y軸周り
	//float ZX = sqrt((velocity_.z * velocity_.z) + (velocity_.x * velocity_.x));

	world_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	//横方向の長さ
	world_.translation_.y *= world_.rotation_.y;
	// x軸周り
	world_.rotation_.x = std::atan2(-velocity_.y, velocity_.z);
#pragma endregion
};

void EnemyBullet::Update() {
	// 座標移動
	world_.translation_.x -= velocity_.x;
	world_.translation_.y -= velocity_.y;
	world_.translation_.z -= velocity_.z;

	// 時間経過で判断する
	--deathTimer_;
	if (deathTimer_ <= 0) {
		isDead_ = true;
	}

	world_.UpdateMatrix();
	// 行列更新
	world_.matWorld_ = MakeAffineMatrix(world_.scale_, world_.rotation_, world_.translation_);
};

void EnemyBullet::Draw(ViewProjection& view) { model_->Draw(world_, view, textureHandle_); }