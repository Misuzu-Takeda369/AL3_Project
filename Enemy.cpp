#include "Enemy.h"
#include "ImGuiManager.h"
#include <cassert>

void Enemy::Initialize(Model* model) {

	
	// ヌルじゃないか確認
	assert(model);
	// 外から貰ってきたデータの受け渡し
	model_ = model;
	textureHandle_ = TextureManager::Load("Pbase.png");

	//  ワールドトランスフォーム初期化(プレイヤーに移動するカメラ個体ごと)
	worldTransform_.Initialize();
	
}


void Enemy::Update() {
	/*
	Vector3 move = {};

	const float kCharactorspeed = 0.4f;
	
	// 入力処理
	move.z -= kCharactorspeed;

	// 敵の移動フェーズ毎の行動
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;
	*/

	switch (phase_) {
	case Enemy::Phase::Approach:
		//移動関数入れる
		ApproachMove();

		break;
	case Enemy::Phase::Leave:

		LeaveMove();
		break;
	
	}
	// 転送
	worldTransform_.TransferMatrix();
	// 行列更新
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	

}


void Enemy::Draw(ViewProjection viewprojection) {

	model_->Draw(worldTransform_, viewprojection, textureHandle_);
}



void Enemy::ApproachMove() {
	Vector3 move = {};

	const float kCharactorspeed = 0.2f;

	// 入力処理
	move.z -= kCharactorspeed;

	// 敵の移動フェーズ毎の行動
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;
	

	if (worldTransform_.translation_.z < -20.0f) {

		phase_ = Enemy::Phase::Leave;
	}
}

void Enemy::LeaveMove() {
	Vector3 move = {};

	const float kCharactorspeed = 0.2f;

	// 入力処理
	move.x -= kCharactorspeed;
	move.y -= kCharactorspeed;

	// 敵の移動フェーズ毎の行動
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;
}