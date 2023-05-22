#include "Enemy.h"
#include "ImGuiManager.h"
#include <cassert>

Enemy::~Enemy() {
	for (EnemyBullet* bullet : bullets_) {

		delete bullet;
	}
}
void Enemy::Initialize(Model* model) {

	// ヌルじゃないか確認
	assert(model);
	// 外から貰ってきたデータの受け渡し
	model_ = model;
	textureHandle_ = TextureManager::Load("Pbase.png");

	//  ワールドトランスフォーム初期化(プレイヤーに移動するカメラ個体ごと)
	worldTransform_.translation_ = {20.0f, 0.0f, 40.0f};
	worldTransform_.Initialize();

	// 接近フェーズ初期化
	ApproachInt();
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

	//Fire();



	switch (phase_) {
	case Enemy::Phase::Approach:
		// 移動関数入れる
		ApproachMove();

		break;
	case Enemy::Phase::Leave:

		LeaveMove();
		break;
	}
	// 転送
	worldTransform_.TransferMatrix();
	// 行列更新
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	for (EnemyBullet* bullet : bullets_) {

		bullet->Update();
	}
}

void Enemy::Draw(ViewProjection viewprojection) {

	model_->Draw(worldTransform_, viewprojection, textureHandle_);

	for (EnemyBullet* bullet : bullets_) {

		bullet->Draw(viewprojection);
	}
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

	if (worldTransform_.translation_.z < 0.0f) {

		phase_ = Enemy::Phase::Leave;
	}

#pragma region 弾
	//発射タイマーカウント(減らす)
	fireTimer_--;
	//指定時間に達した時
	if (fireTimer_ < 0) {
		//発射する
		Fire();
		//タイマーを戻す
		fireTimer_ = kFireInterval;
	}
#pragma endregion
}

void Enemy::LeaveMove() {
	Vector3 move = {};

	const float kCharactorspeed = -0.2f;

	// 入力処理
	move.x += kCharactorspeed;
	move.y -= kCharactorspeed;

	// 敵の移動フェーズ毎の行動
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;
}

void Enemy::Fire() {
	
		// 弾の速度
		const float kBulletSpeed = -2.0f;
		Vector3 velocity(0.0f, 0.0f, kBulletSpeed);

		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		// 弾を登録する
		// bullet_ = newBullet;
		bullets_.push_back(newBullet);
	
}

void Enemy::ApproachInt() 
{ 
	fireTimer_ = kFireInterval;
}