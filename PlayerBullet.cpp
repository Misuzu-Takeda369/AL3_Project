#include "PlayerBullet.h"

#include <cassert>
void PlayerBullet::Initialize(Model* model, const Vector3& position) {

	// ヌルポインター
	assert(model);
	model_ = model;

	textureHandle_ = TextureManager::Load("white1x1.png");

	world_.Initialize();
	world_.translation_ = position;
};

void PlayerBullet::Update() { world_.UpdateMatrix(); };

void PlayerBullet::Draw(ViewProjection& view) { model_->Draw(world_, view, textureHandle_); }