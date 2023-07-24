﻿#include "PlayerBullet.h"
#include "CollistionConfig.h"

#include <cassert>
void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {

	//引数で受け取った速度をメンバ変数に入れる
	velocity_ = velocity;

	// ヌルポインター
	assert(model);
	model_ = model;

	textureHandle_ = TextureManager::Load("white1x1.png");

	//初期化と初期位置を貰う
	world_.Initialize();
	world_.translation_ = position;

	// プレイヤーの属性の設定
	SetCollistionAttribute(kConllistionAttributePlayer);
	// 判定対象の属性を設定する
	SetCollistionMask(kConllistionAttributeEnemy);
};

void PlayerBullet::Update() 
{ 
	//座標移動
	world_.translation_.x += velocity_.x;
	world_.translation_.y += velocity_.y;
	world_.translation_.z += velocity_.z;

	//時間経過で判断する
	--deathTimer_;
	if (deathTimer_<= 0) {
		isDead_ = true;
	}

	world_.UpdateMatrix(); 
};

void PlayerBullet::Draw(ViewProjection& view) {

	model_->Draw(world_, view, textureHandle_);
}


// 当たったことが伝わったらこっちで処理する関数
void PlayerBullet::OnCollision() { isDead_ = true; }

Vector3 PlayerBullet::GetWorldPosition() {
	// ワールドを入れる奴
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = world_.translation_.x;
	worldPos.y = world_.translation_.y;
	worldPos.z = world_.translation_.z;

	return worldPos;
}