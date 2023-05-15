#include "PlayerBullet.h"

#include <cassert>
void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {

	//引数で受け取った速度をメンバ変数に入れる
	velocity_ = velocity;

	// ヌルポインター
	assert(model);
	model_ = model;

	textureHandle_ = TextureManager::Load("white1x1.png");

	world_.Initialize();
	world_.translation_ = position;
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

void PlayerBullet::Draw(ViewProjection& view) { model_->Draw(world_, view, textureHandle_); }